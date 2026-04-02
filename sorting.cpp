#include "sorting.h"



void QuickSort(PagedArray& arr, long long low, long long high){
    if (low >= high) return;
    
    int pivot = arr[high];
    long long lt = low;    // arr[low..lt-1] < pivot
    long long gt = high;   // arr[gt+1..high] > pivot
    long long i = low;
    
    while (i <= gt){
        if (arr[i] < pivot){
            int tmp = arr[lt];
            arr[lt] = arr[i];
            arr[i] = tmp;
            lt++;
            i++;
        } else if (arr[i] > pivot){
            int tmp = arr[gt];
            arr[gt] = arr[i];
            arr[i] = tmp;
            gt--;
        } else {
            i++;
        }
    }
    // arr[lt..gt] son todos iguales al pivote, ya están en su lugar
    QuickSort(arr, low, lt - 1);
    QuickSort(arr, gt + 1, high);
}

void insertionSort(PagedArray& arr, long long low, long long high){
    for (long long i = low + 1; i <= high; i++){
        int key = arr[i];
        long long j = i - 1;
        while (j >= low && arr[j] > key){
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void selectionSort(PagedArray& arr, long long low, long long high){
    for (long long i = low; i < high; i++){
        long long minIdx = i;
        for (long long j = i + 1; j <= high; j++){
            if (arr[j] < arr[minIdx])
                minIdx = j;
        }
        int tmp = arr[minIdx];
        arr[minIdx] = arr[i];
        arr[i] = tmp;
    }
}

void shellSort(PagedArray& arr, long long low, long long high){
    long long n = high - low + 1;
    for (long long gap = n/2; gap > 0; gap /= 2){
        for (long long i = low + gap; i <= high; i++){
            int tmp = arr[i];
            long long j = i;
            while (j >= low + gap && arr[j - gap] > tmp){
                arr[j] = arr[j - gap];
                j -= gap;
            }
            arr[j] = tmp;
        }
    }
}

void mergeSort(PagedArray& arr, long long low, long long high){
    if (low >= high) return;
    long long mid = low + (high - low) / 2;
    mergeSort(arr, low, mid);
    mergeSort(arr, mid + 1, high);
    // merge
    long long n = high - low + 1;
    int* tmp = new int[n];
    long long i = low, j = mid + 1, k = 0;
    while (i <= mid && j <= high){
        if (arr[i] <= arr[j]) tmp[k++] = arr[i++];
        else tmp[k++] = arr[j++];
    }
    while (i <= mid) tmp[k++] = arr[i++];
    while (j <= high) tmp[k++] = arr[j++];
    for (long long i = 0; i < n; i++)
        arr[low + i] = tmp[i];
    delete[] tmp;
}