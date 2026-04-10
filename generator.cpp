#include<cstdio>
#include<cstdlib> 
#include<ctime> 
#include<iostream> 
#include<cstring> 
#include<random> 

 

using ll = long long;

const ll small = 32LL * (1024LL * 1024LL)/sizeof(int); 
const ll medium =  64LL * (1024LL * 1024LL)/sizeof(int);
const ll large = 128LL *(1024LL * 1024LL)/sizeof(int);
const ll tiny = 18LL * 1024LL /sizeof(int); 


void escritura(const std::string path,const ll size){
    //fopen es una funcion tipo C, no conoce string, entonces hay que pasar 
    //path a char* con el uso de c_str. 
    FILE* f = fopen(path.c_str(), "wb"); 
    if (!f){
        std::cout << "error al crear el archivo\n"; 
        return; 
    }

    const int buffer_size = 1024 * 1024; 
    //new int[size] = pide memoria dinamica en el heap para size enteros. 
    //type* p = new type[size] puntero al inicio de la memoria pedida. 

    int* buffer = new int[buffer_size]; 

    std::mt19937 rng(time(nullptr));
    std::uniform_int_distribution<int> dist(INT_MIN, INT_MAX);

    
    ll remaining = size; 

    while(remaining > 0){
        int chunk = (remaining < buffer_size) ? remaining : buffer_size; 
        for (int i = 0; i < chunk; i++){
            buffer[i] = dist(rng);
        }
        fwrite(buffer, sizeof(int), chunk, f); 
        remaining -= chunk;
    }
    //limpia esa memoria: 
    delete[] buffer;
    fclose(f); 
    std:: cout << "archivo generado: " << path << "\n"; 
}

int main(int argc, char* argv[]){
    //char* = localizacion al primer caracter,
    //imprime 

    //inicializacion de la creacion de archivos binarios: 
    std::string size_str = ""; 
    std::string output_path =""; 
    for (int i = 1; i < argc - 1 ; i++){
        if (strcmp(argv[i], "-size") == 0){
            size_str = argv[i + 1]; 
        } 

        if (strcmp(argv[i], "-output") == 0){
            output_path = argv[i + 1];

        }
    }
    //validacion de input
    if (size_str.empty() || output_path.empty()){
        std::cout << "input invalido\n"; 
        return 1; 
    }
    //seleccion de tamano. 
    ll size = 0; 
    if (size_str == "SMALL"){
        size = small; 
    } else if (size_str == "MEDIUM"){
        size = medium;
    } else if (size_str == "LARGE"){
        size = large; 
    } else if (size_str == "TINY"){
        size = tiny; 
    } else {
            std:: cout << "tamano invalido\n"; 
            return 1; 
    }

    //escritura: 
    escritura(output_path, size); 
    
}
