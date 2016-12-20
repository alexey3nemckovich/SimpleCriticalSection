#include "stdafx.h"
#include "CriticalSection.h"


CriticalSection::CriticalSection(short *sectionFlag)
{
    this->sectionAvailable = sectionFlag;
}


CriticalSection::~CriticalSection()
{

}


bool CriticalSection::TryEnter()
{
    return InterlockedCompareExchange16(sectionAvailable, false, true);
}


void CriticalSection::Enter()
{
    while (!TryEnter())
    {
        std::this_thread::yield();
    }
}


void CriticalSection::Leave()
{
    InterlockedExchange16(sectionAvailable, true);
}