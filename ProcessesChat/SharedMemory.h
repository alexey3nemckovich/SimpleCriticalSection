#pragma once
#include <string>
using namespace std;


class SharedMemory
{
private:
    CRITICAL_SECTION criticalSection;
    const int sharedMemSize = 100;
    const LPCWSTR sharedMemName = L"ProcessesSharedMemory";
    HANDLE hMapFile;
    void *sharedDataPointer;
    char *dataPointer;
public:
    SharedMemory();
    ~SharedMemory();
    void write(char *buff);
    void read(char* &buff);
};