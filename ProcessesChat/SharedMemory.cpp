#include "stdafx.h"
#include "SharedMemory.h"


LPCWSTR CharToLPCWSTR(const char* str)
{
    return nullptr;
}


SharedMemory::SharedMemory()
{
    sharedMemCreator = false;
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
        int errCode = GetLastError();
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
        mt = CreateMutex(NULL, NULL, NULL);
        t tt;
        tt.hMutex = mt;
        CopyMemory(sharedDataPointer, &tt, sizeof(t));
        /*InitializeCriticalSection((CRITICAL_SECTION*)sharedDataPointer);*/
    }
    else
    {
        mt = ((t*)sharedDataPointer)->hMutex;
    }
    /*criticalSection = *((CRITICAL_SECTION*)sharedDataPointer);*/
    dataPointer = (char*)sharedDataPointer + sizeof(mutex);
}


SharedMemory::~SharedMemory()
{
    if (sharedMemCreator)
    {
        //free handle
    }
    UnmapViewOfFile(sharedDataPointer);
    CloseHandle(hMapFile);
}


void SharedMemory::write(char *str)
{
    static int writeCount;
    /*EnterCriticalSection(&criticalSection);*/
    WaitForSingleObject(mt, INFINITE);
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
    ReleaseMutex(mt);
    //PulseEvent(mt);
    /*LeaveCriticalSection(&criticalSection);*/
}


void SharedMemory::read(char* &buff)
{
    static int readCount;
    /*EnterCriticalSection(&criticalSection);*/
    WaitForSingleObject(mt, INFINITE);
    if (strlen(dataPointer))
    {
        strcpy(buff, dataPointer);
        dataPointer += strlen(dataPointer) + 1;
    }
    else
    {
        strcpy(buff, "");
    }
    ReleaseMutex(mt);
    //PulseEvent(mt);
    /*LeaveCriticalSection(&criticalSection);*/
}