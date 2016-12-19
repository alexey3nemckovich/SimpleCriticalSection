#include "stdafx.h"
#include "SharedMemory.h"


LPCWSTR CharToLPCWSTR(const char* str)
{
    return nullptr;
}


SharedMemory::SharedMemory()
{
    bool sharedMemCreator = false;
    hMapFile = OpenFileMapping(
        FILE_MAP_ALL_ACCESS,
        FALSE,     
        sharedMemName
    );
    if (hMapFile == NULL)
    {
        hMapFile = CreateFileMapping(
            INVALID_HANDLE_VALUE, 
            NULL,           
            PAGE_READWRITE,  
            0,                     
            sharedMemSize,              
            sharedMemName
        );
        sharedMemCreator = true;
    }
    sharedDataPointer = MapViewOfFile(
        hMapFile,
        FILE_MAP_ALL_ACCESS,
        0,
        0,
        sharedMemSize
    );
    if (sharedMemCreator)
    {
        ZeroMemory(sharedDataPointer, sharedMemSize);
    }
    dataPointer = (char*)sharedDataPointer;
    InitializeCriticalSection(&criticalSection);
}


SharedMemory::~SharedMemory()
{
    UnmapViewOfFile(sharedDataPointer);
    CloseHandle(hMapFile);
}


void SharedMemory::write(char *str)
{
    static int writeCount;
    EnterCriticalSection(&criticalSection);
    writeCount = strlen(str) + 1;
    if (writeCount > sharedMemSize)
    {
        printf("Message is too large to write it to processes shared memory!\n");
    }
    else
    {
        if ((dataPointer + writeCount) > ((char*)sharedDataPointer + sharedMemSize))
        {
            ZeroMemory(sharedDataPointer, sharedMemSize);
            dataPointer = (char*)sharedDataPointer;
        }
        CopyMemory(dataPointer, str, writeCount);
        dataPointer += writeCount;
    }
    LeaveCriticalSection(&criticalSection);
}


void SharedMemory::read(char* &buff)
{
    static int readCount;
    EnterCriticalSection(&criticalSection);
    if (strlen(dataPointer))
    {
        strcpy(buff, dataPointer);
        dataPointer += strlen(dataPointer) + 1;
    }
    else
    {
        strcpy(buff, "");
    }
    LeaveCriticalSection(&criticalSection);
}