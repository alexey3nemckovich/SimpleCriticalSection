#include "stdafx.h"
#include "SharedMemory.h"
#include <cstdlib>


void PCharToLPCWSTR(const char *pChar, LPCWSTR lpStr)
{
    mbstowcs((wchar_t*)lpStr, pChar, strlen(pChar) + 1);
}


SharedMemory::SharedMemory(string name, int size)
{
    this->size = size;
    this->name = name;
    sharedMemCreator = false;
    LPCWSTR lpName = (LPCWSTR)malloc(100);
    PCharToLPCWSTR(name.c_str(), lpName);
    hMapFile = GetHandle(lpName);
    if (hMapFile == NULL)
    {
        hMapFile = Create(lpName, size);
        sharedMemCreator = true;
    }
    free((void*)lpName);
    sharedDataPointer = GetDataPointer(hMapFile, size);
    short *shortDataPointer = (short*)sharedDataPointer;
    if (sharedMemCreator)
    {
        ZeroMemory(sharedDataPointer, size);
        short countProcesses = 0;
        short sectionFree = true;
        CopyMemory(&(shortDataPointer[0]), &countProcesses, sizeof(short));
        CopyMemory(&(shortDataPointer[1]), &sectionFree, sizeof(short));
    }
    countLiveProcesses = &(shortDataPointer[0]);
    InterlockedIncrement16(countLiveProcesses);
    criticalSection = new CriticalSection(&(shortDataPointer[1]));
    dataPointer = (char*)&(shortDataPointer[2]);
}


SharedMemory::~SharedMemory()
{
    InterlockedDecrement16(countLiveProcesses);
    if (InterlockedCompareExchange16(countLiveProcesses, 0, 0) == 0)
    {
        CloseHandle(hMapFile);
    }
    UnmapViewOfFile(sharedDataPointer);
}


void SharedMemory::write(char *str)
{
    static int writeCount;
    criticalSection->Enter();
    writeCount = strlen(str) + 1;
    if (writeCount > size)
    {
        printf("Message is too large to write it to processes shared memory!\n");
    }
    else
    {
        if ((dataPointer + writeCount) > ((char*)sharedDataPointer + size))
        {
            ZeroMemory(sharedDataPointer, size);
            dataPointer = (char*)sharedDataPointer;
        }
        CopyMemory(dataPointer, str, writeCount);
        dataPointer += writeCount;
    }
    criticalSection->Leave();
}


void SharedMemory::read(char* &buff)
{
    static int readCount;
    criticalSection->Enter();
    if (strlen(dataPointer))
    {
        strcpy(buff, dataPointer);
        dataPointer += strlen(dataPointer) + 1;
    }
    else
    {
        strcpy(buff, "");
    }
    criticalSection->Leave();
}


HANDLE SharedMemory::GetHandle(LPCWSTR sharedMemName)
{
    return OpenFileMapping(
        FILE_MAP_ALL_ACCESS,
        FALSE,
        sharedMemName
    );
}


HANDLE SharedMemory::Create(LPCWSTR sharedMemName, int size)
{
    return CreateFileMapping(
        INVALID_HANDLE_VALUE,
        NULL,
        PAGE_READWRITE,
        0,
        size,
        sharedMemName
    );
}


void *SharedMemory::GetDataPointer(HANDLE hShMemory, int size)
{
    return MapViewOfFile(
        hShMemory,
        FILE_MAP_ALL_ACCESS,
        0,
        0,
        size
    );
}