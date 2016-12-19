#pragma once
#include <string>
#include <mutex>
using namespace std;


struct t
{
    HANDLE hMutex;
};


class SharedMemory
{
private:
    bool sharedMemCreator;
    HANDLE mt;
    CRITICAL_SECTION criticalSection;
    const int sharedMemSize = 100;
    const LPCWSTR sharedMemName = L"Global\\SharedMemory";
    HANDLE hMapFile;
    void *sharedDataPointer;
    char *dataPointer;
public:
    SharedMemory();
    ~SharedMemory();
    void write(char *buff);
    void read(char* &buff);
};