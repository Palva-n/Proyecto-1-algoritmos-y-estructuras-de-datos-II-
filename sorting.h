#ifndef SORTING_H
#define SORTING_H 
#include "PagedArray.h"

//se aguantan bastantes datos: 
void QuickSort(PagedArray& arr, long long low, long long hi); 

//para seleccion limitada: 
void mergeSort(PagedArray& arr, long long low, long long high);
void shellSort(PagedArray& arr, long long low, long long high);
void insertionSort(PagedArray& arr, long long low, long long high);
void selectionSort(PagedArray& arr, long long low, long long high);

#endif 