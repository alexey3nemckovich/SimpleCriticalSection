#include "stdafx.h"
#include "ProcessesChat.h"


int main()
{
    ProcessesChat *pcChat = new ProcessesChat();
    pcChat->StartChat();
    delete(pcChat);
}