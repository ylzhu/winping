#include "StdAfx.h"
#include "ProtoInfo.h"

CProtoInfo::CProtoInfo(void)
{
}

CProtoInfo::~CProtoInfo(void)
{
}

struct addrinfo* CProtoInfo::ResolveAddress(char* addr, char* port, int af, int type, int proto)
{
	struct addrinfo hints;
	struct addrinfo* res = NULL;
	int rc = 0;
	memset(&hints, 0, sizeof(hints));
	hints.ai_flags = (NULL != addr ? 0 : AI_PASSIVE);
	hints.ai_family = af;
	hints.ai_socktype = type;
	hints.ai_protocol = proto;
	if(0 != getaddrinfo(addr, port, &hints, &res))
	{
		return NULL;
	}
	else
	{
		return res;
	}
}

int CProtoInfo::SetTTL(SOCKET s, int ttl)
{	
	return setsockopt(s, IPPROTO_IP, IP_TTL, (char*)&ttl, sizeof(ttl));
}

void CProtoInfo::InitIcmpHeader(char* buf, int datasize)
{
	ICMP_HDR* icmp_hdr = NULL;
	char* datapart = NULL;
	icmp_hdr = (ICMP_HDR*)buf;
	icmp_hdr->icmp_type = ICMPV4_ECHO_REQUEST_TYPE;
	icmp_hdr->icmp_code = ICMPV4_ECHO_REQUEST_CODE;
	icmp_hdr->icmp_id = (USHORT)GetCurrentProcessId();
	icmp_hdr->icmp_checksum = 0;
	icmp_hdr->icmp_sequence = 0;
	datapart = buf + sizeof(ICMP_HDR);
	memset(datapart, 'E', datasize);
}

USHORT CProtoInfo::Checksum(USHORT *buffer, int size)
{
	unsigned long cksum = 0;
	while(size > 1)
	{
		cksum += *buffer++;
		size -= sizeof(USHORT);
	}
	if(size)
	{
		cksum += *(UCHAR*)buffer;
	}
	cksum = (cksum >> 16) + (cksum & 0xFFFF);
	cksum += (cksum >> 16);
	return (USHORT)(~cksum);
}

void CProtoInfo::SetIcmpSequence(char* buf)
{
	unsigned long sequence = GetTickCount();
	ICMP_HDR* icmp = (ICMP_HDR*)buf;
	icmp->icmp_sequence = (USHORT)sequence;
}

void CProtoInfo::ComputeIcmpChecksum(char* buf, int packetlen)
{
	ICMP_HDR* icmp = (ICMP_HDR*)buf;
	icmp->icmp_checksum = 0;
	icmp->icmp_checksum = Checksum((USHORT*)buf, packetlen);
}

CString CProtoInfo::PrintAddress(SOCKADDR* sa, int salen)
{
	CString strText(_T("")); 
	char host[NI_MAXHOST] = {0};
	char serv[NI_MAXSERV] = {0};
	int hostlen = NI_MAXHOST;
	int servlen = NI_MAXSERV;
	if(0 != getnameinfo(sa, salen, host, hostlen, serv, servlen, NI_NUMERICHOST | NI_NUMERICSERV))
	{
		return _T("");
	}
	CString strHost(host);
	CString strServ(serv);

	if(0 != strcmp(serv, "0"))
	{
		strText.Format(_T("[%s]:%s"), strHost, strServ);
	}
	else
	{
		strText.Format(_T("%s"), strHost);
	}
	return strText;
}