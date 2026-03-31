#include "PagedArray.h"
#include<cstdio> 
#include<string>
#include<stdexcept> 
#include<iostream>
PagedArray::PagedArray(std::string file_path, int pgsz, int pgcnt){
    //acceder al archivo: 
    file = fopen(file_path.c_str(), "r+b"); 
    //si el archivo no esta abierto tirar un error de tiempo de compilacion: 
    if (!file){
        throw std::runtime_error("No se pudo abrir el archivo"); 
    }
    //inicializacion de paginas: 
    pageSize = pgsz; 
    pageCount = pgcnt; 
    pageFaults = 0; 
    pageHits = 0; 
    fifoCounter = 0; 
    
    //ranuras de paginas: 
    slots = new PageSlot[pageCount]; 
    for (long long i = 0; i < pageCount; i++){
        //data de cada pagina: 
        slots[i].data = new int[pageSize]; 
        //pagina vacia: 
        slots[i].pagedId = -1; 
        //sin cambiar: 
        slots[i].dirty = false; 
        //sin ningun orden impuesto. 
        slots[i].fifoOrder = -1; 
    }
    //llevar el puntero al final
    fseek(file, 0, SEEK_END);
    //recuperar su valor de posicion en bytes. 
    long long bytes = ftell(file);
    //tamanio de archivo: 
    totalsize = bytes/sizeof(int); 
}
PagedArray::~PagedArray(){
    //cerrar el archivo: 
    for (long long i = 0; i < pageCount; i++){
        if (slots[i].pagedId != -1 && slots[i].dirty){
            writePage(i); 
        }
        delete[] slots[i].data; 
    }
    //destruir paginas: 
    delete[] slots; 
    fclose(file); 
}
long long PagedArray::size(){
    return totalsize; 
}
int& PagedArray::operator[](long long i){
    static long long countOp = 0; 
    countOp++;
    
    long long pageId = i/pageSize; 
    long long offset = i%pageSize; 
    //entonces el entero i estaria en la pagina i/pgsz, en la posicion i%pagesize
    

    long long slot = findSlot(pageId); 
    if (slot == -1){
        pageFaults++; 
        slot = evictFIFO(); 
        loadPage(slot, pageId);
    } else {
        pageHits++; 
    }
    //si slot = -1 entonces la pagina ya estaba cargada. 
    int& ref = slots[slot].data[offset]; 
    slots[slot].dirty = true; 
    return ref; 
}
long long PagedArray::findSlot(long long pageId){
    for (long long i = 0; i < pageCount; i++){
        if (slots[i].pagedId == pageId){
            return i;
        }
    }
    return -1; 
} 
int PagedArray::evictFIFO(){
    int oldest = 0; 
    for (int i = 0; i < pageCount; i++){
        if(slots[i].fifoOrder < slots[oldest].fifoOrder){
            oldest = i; 
        }
    }
    return oldest; 
}
void PagedArray::writePage(int slotIndex){
    long long pageId = slots[slotIndex].pagedId;
    //calcula a partir de donde escribe: 
    long long offset = pageSize * sizeof(int) * pageId;  
    //cuantos enteros realmente tiene esta pagina:
    long long count = pageSize;
    if (pageId * pageSize + pageSize > totalsize)
        count = totalsize - pageId * pageSize;
    //lleva el puntero alli: 
    fseek(file, offset, SEEK_SET);
    //escribe: 
    fwrite(slots[slotIndex].data, sizeof(int), count, file); 
    //hace notificar que este slot de pagina no ha sido modificado
    //desde su ultima escritura: 
    slots[slotIndex].dirty = false; 
}
void PagedArray::loadPage(int slotIndex, long long pageIndex){
    if (slots[slotIndex].dirty){
        writePage(slotIndex); 
    }
    long long byteoffset = pageIndex * sizeof(int) * pageSize; 
    //cuantos enteros realmente tiene esta pagina:
    long long count = pageSize;
    if (pageIndex * pageSize + pageSize > totalsize)
        count = totalsize - pageIndex * pageSize;
    fseek(file, byteoffset, SEEK_SET);
    fread(slots[slotIndex].data, sizeof(int), count, file);
    slots[slotIndex].pagedId = pageIndex; 
    slots[slotIndex].dirty = false; 
    slots[slotIndex].fifoOrder = fifoCounter++; 
}

long long PagedArray::getPageFaults(){
    return pageFaults;
}
long long PagedArray::getPageHits(){
    return pageHits;
}