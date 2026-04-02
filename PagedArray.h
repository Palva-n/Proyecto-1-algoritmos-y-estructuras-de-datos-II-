#ifndef PAGED_ARRAY_H
#define PAGED_ARRAY_H
#include<cstdio>
#include<string> 
class PagedArray{
    public: 
        
        PagedArray(std::string file_path, int pgsz, int pgcnt); 
        ~PagedArray(); 
        long long size(); 
        int& operator[] (long long i); 
        long long getPageFaults(); 
        long long getPageHits(); 
    private: 
        struct PageSlot{
            int* data; //misma vaina que array por lo de p[i] = *(p + i) 
            long long pagedId; 
            bool dirty; //para saber si se modifico
            long long fifoOrder; 
        };
        PageSlot* slots; 
        long long pageSize; 
        long long pageCount; 
        long long pageFaults; 
        long long pageHits; 
        long long fifoCounter; 
        FILE* file;
        long long totalsize;
        long long findSlot(long long pageId); 
        int evictFIFO(); 
        void writePage(int slotIndex); 
        void loadPage(int slotIndex, long long pageId); 
};
#endif