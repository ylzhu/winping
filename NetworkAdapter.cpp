// MacAddress.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <windows.h>
#include <wincon.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <Nb30.h>
#include "NetworkAdapter.h"
#pragma comment(lib,"netapi32.lib")  



#include <winsock2.h>
#include <Iphlpapi.h>
#include <stdio.h>
#pragma   comment(lib,   "iphlpapi.lib") 
 
void byte2Hex(unsigned char bData,char hex[])
{
    int high=bData/16,low =bData %16;
    hex[0] = (high <10)?('0'+high):('A'+high-10);
    hex[1] = (low <10)?('0'+low):('A'+low-10);
}
 
int CNetworkAdaper::getLocalMac(char *description, char *mac) //获取本机MAC址 
{
    ULONG ulSize=0;
    PIP_ADAPTER_INFO pInfo=NULL;
    int temp=0;
	m_bGetMacStatus_Idle = FALSE;
	m_bGetMacSuccess = FALSE;
    temp = GetAdaptersInfo(pInfo,&ulSize);//第一处调用，获取缓冲区大小
    pInfo=(PIP_ADAPTER_INFO)malloc(ulSize);
    temp = GetAdaptersInfo(pInfo,&ulSize);
 
    int iCount=0;
    while(pInfo)//遍历每一张网卡
    {
        //  pInfo->Address MAC址
		if (strstr(pInfo->Description, description) != NULL)
        {
	        for(int i=0;i<(int)pInfo->AddressLength;i++)
	        {
	            byte2Hex(pInfo->Address[i],&mac[iCount]);
	            iCount+=2;
	            if(i<(int)pInfo->AddressLength-1)
	            {
	                mac[iCount++] = ':';
	            }else
	            {
	                mac[iCount++] = '#';
	            }
	        }
			break;
        }
        pInfo = pInfo->Next;
    }
	
 	m_bGetMacStatus_Idle = TRUE;
	
    if(iCount >0)
    {
		m_bGetMacSuccess = TRUE;
        mac[iCount]='\0';
        return iCount;
    }
    else return -1;
}

int CNetworkAdaper::getDevice(char *description) //获取本机MAC址 
{
    ULONG ulSize=0;
    PIP_ADAPTER_INFO pInfo=NULL;
	int ret = 0;
    int temp=0;
    temp = GetAdaptersInfo(pInfo,&ulSize);//第一处调用，获取缓冲区大小
    pInfo=(PIP_ADAPTER_INFO)malloc(ulSize);
    temp = GetAdaptersInfo(pInfo,&ulSize);
 	// Description = 0x00375e1c "Realtek RTL8188ETV Wireless LAN 802.11n USB 2.0 Network Adapter"
    while(pInfo)//遍历每一张网卡
    {
        //  pInfo->Address MAC址
		if (strstr(pInfo->Description, description) != NULL)
        {
	        ret = 1;
			break;
        }
        pInfo = pInfo->Next;
    }
	
   
    return ret;
}

