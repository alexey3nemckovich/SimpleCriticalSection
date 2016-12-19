#pragma once
#include <thread>
#include <iostream>
#include "SharedMemory.h"
using namespace std;

class ProcessesChat
{
private:
    bool online;
    thread listenThread;
    SharedMemory *sharedMem;
    friend void ListenMessages(ProcessesChat *pcChat);
public:
    ProcessesChat();
    ~ProcessesChat();
    void StartChat();
};