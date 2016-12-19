#pragma once


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