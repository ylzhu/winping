#pragma once
#include <windows.h>

class CNetworkAdaper
{
public:
	CNetworkAdaper(){ m_bGetMacStatus_Idle = TRUE; m_bGetMacSuccess = FALSE;};
    ~CNetworkAdaper(){};
	int getLocalMac(char *ip, char *mac);
    //int GetMac(char * mac);
public:
    bool m_bGetMacStatus_Idle;
    bool m_bGetMacSuccess;
};
