#pragma once
#include <string>
#include <mutex>
#include <atomic>
#include "CriticalSection.h"
using namespace std;


class SharedMemory
{
private:
    //fields
    int size;
    string name;
    HANDLE hMapFile;
    char *dataPointer;
    bool sharedMemCreator;
    void *sharedDataPointer;
    short *countLiveProcesses;
    CriticalSection *criticalSection;
    //methods
    static HANDLE GetHandle(LPCWSTR lpSharedMemName);
    static HANDLE Create(LPCWSTR lpSharedMemName, int size);
    static void* GetDataPointer(HANDLE hShMemory, int size);
public:
    SharedMemory(string name, int size);
    ~SharedMemory();
    void write(char *buff);
    void read(char* &buff);
};