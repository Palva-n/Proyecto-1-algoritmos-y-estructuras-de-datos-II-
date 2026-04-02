#include "sorting.h"
#include<string>
#include <iostream>
#include<chrono> 


int copy(std::string input, std::string output){
    
    FILE* f = fopen(input.c_str(), "rb"); 
    FILE* fp = fopen(output.c_str(), "wb"); 
    if (!f || !fp){
        return -1; 
    }
    long long buffer_size = 1024 * 1024; 
    int* buffer = new int[buffer_size];
    fseek(f, 0, SEEK_END);

    long long bytes = ftell(f);
    long long size = bytes/sizeof(int); 
    fseek(f, 0, SEEK_SET);
    long long remaining = size; 

    while (remaining > 0){
        long long chunk = (remaining > buffer_size) ?  buffer_size : remaining;
        
        fread(buffer, sizeof(int) ,chunk, f);
        fwrite(buffer, sizeof(int), chunk, fp); 
        
        remaining -= chunk;  
    }

    delete[] buffer; 
    fclose(f); 
    fclose(fp); 
    return 0; 
}
void creador_txt(std::string input, int to_read){
    std::string output_txt = input.substr(0, input.find_last_of('.')) + ".txt";
    FILE* f = fopen(input.c_str(), "rb"); 
    FILE* txt = fopen(output_txt.c_str(), "w");
    if (!f){
        std:: cout << "PROBLEMAS CON EL INPUT, NO SE PUDO ABRIR\n"; 
        return; 
    } 
    if (!txt){
        std::cout << "PROBLEMAS CON EL OUTPUT, NO SE PUDO GENERAR\n"; 
        return; 
    }
    for (int i = 0; i < to_read; i++){
        int val; 
        fread(&val, sizeof(int), 1, f); 
        fprintf(txt, "%d, ", val);
    }

    fclose(f); 
    fclose(txt); 
    std::cout << "Exito, txt generado en: " << output_txt << "\n"; 
    return; 

}
int main(int argc, char* argv[]){
    
    std::string input = "",
    output = "",
    algo = "";

    int pageSize = 0,
    pageCount = 0; 
    for (int i = 1; i <= argc - 1; i++){
        std::string t = argv[i]; 

        if (t == "-input"){
            input = argv[i + 1]; 
            if (input == ""){
                std::cout << "error de formato en el input path\n"; 
                return -1; 
            }
        }
        if (t == "-output"){
            output = argv[i + 1]; 
            if (output == ""){
                std::cout << "error de formato en el output path\n"; 
                return -1; 
            }
        }
        if (t == "-alg"){
            algo = argv[i + 1]; 
            if (algo == ""){
                std::cout << "error de formato en el algoritmo\n"; 
                return -1; 
            }
        } 
        if (t == "-pageSize"){
            pageSize = atoi(argv[i + 1]);
            if (pageSize <= 0){
                std::cout << "error en el formato de pageSize\n"; 
                return 0; 
            }
        } 
        if (t == "-pageCount"){
            pageCount = atoi(argv[i + 1]);
            if (pageCount <= 0){
                std::cout << "error en el formato de pageSize\n"; 
                return 0; 
            }
        }
    }
    if (copy(input,output) == -1){
        std::cout << "error de apertura al copiar\n"; 
        return 1; 
    }
    int t; 
    {
        PagedArray arr = PagedArray(output, pageSize, pageCount);
        t = arr.size(); 
        auto start = std::chrono::high_resolution_clock::now();
        if (algo == "QuickSort"){
            std::cout << "iniciando quicksort, size: " << arr.size() << "\n";
            QuickSort(arr, 0, arr.size() - 1);
        } else if (algo == "insertionSort"){
            std::cout<< "iniciando insertion sort con los primeros 1000 elementos: \n"; 
            insertionSort(arr, 0, (999 > arr.size()) ? arr.size() - 1 : 999);
        } else if (algo == "selectionSort"){
            std::cout<<"iniciando selection Sort con los primeros 1000 elementos"; 
            selectionSort(arr, 0, (999 > arr.size()) ? arr.size() - 1 : 999);
        } else if (algo == "shellSort"){
            std::cout << "iniciando ShellSort, size: " << arr.size() << "\n";  
            shellSort(arr, 0, (999 > arr.size()) ? arr.size() - 1 : 999);
        } else if (algo == "mergeSort"){
            std::cout << "iniciando mergeSort, size: " << arr.size() << "\n";  
            mergeSort(arr, 0, arr.size() - 1);
        } else {
            std::cout << "algoritmo invalido\n";
            return 1;
        }
        auto end = std::chrono::high_resolution_clock::now();
        double elapsed = std::chrono::duration<double>(end - start).count();

        std::cout << pageSize << "," << pageCount << ","
          << arr.getPageHits() << "," 
          << arr.getPageFaults() << ","  
          << elapsed << "\n";

    }
    if (algo == "QuickSort" || algo == "mergeSort"){
        creador_txt(output, (int) 1e5 );
    } else{
        creador_txt(output, (999 > t) ? t : 999); 
    }
}