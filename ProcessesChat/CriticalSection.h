#pragma once
#include <thread>
using namespace std;


class CriticalSection
{
private:
    short *sectionAvailable;
public:
    bool TryEnter();
    void Enter();
    void Leave();
    CriticalSection(short *sectionFlag);
    ~CriticalSection();
};