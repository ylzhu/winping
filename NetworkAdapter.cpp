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

#if 1

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
 
int CNetworkAdaper::getLocalMac(char *ip, char *mac) //获取本机MAC址 
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
        if(strcmp(ip, pInfo->IpAddressList.IpAddress.String) == 0)
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
#else
int CNetworkAdaper::GetMac(char * mac)     
{     
    NCB ncb;
    typedef struct _ASTAT_
    {
        ADAPTER_STATUS   adapt;
        NAME_BUFFER   NameBuff   [30];
    }ASTAT,*PASTAT;

    ASTAT Adapter;   

    typedef struct _LANA_ENUM
    {
        UCHAR   length;
        UCHAR   lana[MAX_LANA];
    }LANA_ENUM;

    LANA_ENUM lana_enum;    
    UCHAR uRetCode;
    memset(&ncb, 0, sizeof(ncb));
    memset(&lana_enum, 0, sizeof(lana_enum));    
    ncb.ncb_command = NCBENUM;
    ncb.ncb_buffer = (unsigned char *)&lana_enum;
    ncb.ncb_length = sizeof(LANA_ENUM);
    uRetCode = Netbios(&ncb);

    if(uRetCode != NRC_GOODRET)     
        return uRetCode;     

    for(int lana=0; lana<lana_enum.length; lana++)     
    {
        ncb.ncb_command = NCBRESET;
        ncb.ncb_lana_num = lana_enum.lana[lana];
        uRetCode = Netbios(&ncb); 
        if(uRetCode == NRC_GOODRET)
            break; 
    }

    if(uRetCode != NRC_GOODRET)
        return uRetCode;     

    memset(&ncb, 0, sizeof(ncb));
    ncb.ncb_command = NCBASTAT;
    ncb.ncb_lana_num = lana_enum.lana[0];
    strcpy((char*)ncb.ncb_callname, "*");
    ncb.ncb_buffer = (unsigned char *)&Adapter;
    ncb.ncb_length = sizeof(Adapter);
    uRetCode = Netbios(&ncb);

    if(uRetCode != NRC_GOODRET)
        return uRetCode;

    sprintf(mac,"%02X-%02X-%02X-%02X-%02X-%02X",
        Adapter.adapt.adapter_address[0],
        Adapter.adapt.adapter_address[1],
        Adapter.adapt.adapter_address[2],
        Adapter.adapt.adapter_address[3],
        Adapter.adapt.adapter_address[4],
        Adapter.adapt.adapter_address[5]);

    return 0;   
} 
#endif

#if 0
int main(int argc, char* argv[])
{
    char   mac[200]; 
    GetMac(mac);   
    printf("The Mac Address is : %s   \n",mac);        

    system("pause");
    return 0;
}
#endif
