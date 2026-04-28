generator: 
generator posee 4 sizes: tiny, small, medium, large de los tamaños respectivos (18 kB, 32 MB, 64 MB, 128MB)


para poder generar un archivo se usa el comando en cmd (windows): 


generator.exe -size SIZE -output OUTPUT


Output tiene que ser de la forma: Name.bin 


Sorter: 


Para poder generar un sorter.exe se usa el comando:


g++ -O2 -o sorter sorter.cpp sorting.cpp PagedArrray.cpp


Para poder ejecutar algoritmos con sorter se usa el siguiente comando: 


sorter.exe -input INPUT -output OUTPUT -alg ALGORITHM -pageSize SIZE -pageCount COUNT. 
Algorithm: quickSort, shellSort, insertionSort,mergeSort, selectionSort
SIZE: SMALL, MEDIUM, LARGE

las palabras en mayusculas son los argumentos.
