#include "stdafx.h"
#include "ProcessesChat.h"


void ListenMessages(ProcessesChat *pcChat);


ProcessesChat::ProcessesChat()
{
    online = true;
    sharedMem = new SharedMemory("Global\\SharedMemory", 1000);
    listenThread = thread(ListenMessages, this);
}


ProcessesChat::~ProcessesChat()
{
    listenThread.join();
    delete(sharedMem);
}


void ProcessesChat::StartChat()
{
    static char str[100];
    do
    {
        printf("Enter message of press 'Enter' with empty string to finish...\n");
        gets_s(str);
        if (strcmp(str, "") != 0)
        {
            sharedMem->write(str);
        }
        else
        {
            online = false;
        }
    } while (online);
}


void ListenMessages(ProcessesChat *pcChat)
{
    char str[100];
    char* s = str;
    while (pcChat->online)
    {
        pcChat->sharedMem->read(s);
        if (strlen(s) != 0)
        {
            printf("Message: %s\n", s);
        }
    }
}