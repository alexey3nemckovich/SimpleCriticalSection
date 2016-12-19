#pragma once
#include <string>
#include <mutex>
using namespace std;


class SharedMemory
{
private:
    bool sharedMemCreator;
    mutex *mt;
    CRITICAL_SECTION criticalSection;
    const int sharedMemSize = 100;
    const LPCWSTR sharedMemName = L"Global\\ProcessesSharedMemory";
    HANDLE hMapFile;
    void *sharedDataPointer;
    char *dataPointer;
public:
    SharedMemory();
    ~SharedMemory();
    void write(char *buff);
    void read(char* &buff);
};