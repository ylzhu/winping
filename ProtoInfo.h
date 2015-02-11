#pragma once

#include <Winsock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "WS2_32")

typedef struct ip_hdr
{
	unsigned char  ip_verlen;
	unsigned char  ip_tos;
	unsigned short ip_totallength;
	unsigned short ip_id; 
	unsigned short ip_offset;
	unsigned char  ip_ttl;
	unsigned char  ip_protocol;
	unsigned short ip_checksum;
	unsigned int   ip_srcaddr;
	unsigned int   ip_destaddr;
}IPV4_HDR, *PIPV4_HDR;

typedef struct ipv4_option_hdr
{    
	unsigned char   opt_code;
	unsigned char   opt_len;
	unsigned char   opt_ptr;
	unsigned long   opt_addr[9];
}IPV4_OPTION_HDR, *PIPV4_OPTION_HDR;

typedef struct icmp_hdr
{    
	unsigned char   icmp_type;
	unsigned char   icmp_code;
	unsigned short  icmp_checksum;
	unsigned short  icmp_id;
	unsigned short  icmp_sequence;
} ICMP_HDR, *PICMP_HDR;

#define IP_RECORD_ROUTE     0x7            
#define ICMPV4_ECHO_REQUEST_TYPE   8
#define ICMPV4_ECHO_REQUEST_CODE   0
#define ICMPV4_ECHO_REPLY_TYPE     0
#define ICMPV4_ECHO_REPLY_CODE     0
#define ICMPV4_MINIMUM_HEADER      8

#define DEFAULT_TTL				   128

class CProtoInfo
{
public:
	CProtoInfo(void);
	virtual ~CProtoInfo(void);

public:
	static struct addrinfo* ResolveAddress(char* addr, char* port, int af, int type, int proto);
	static int SetTTL(SOCKET s, int ttl = DEFAULT_TTL);
	static void InitIcmpHeader(char* buf, int datasize);
	static USHORT Checksum(USHORT* buffer, int size);
	static void SetIcmpSequence(char* buf);
	static void ComputeIcmpChecksum(char* buf, int packetlen);
	static CString PrintAddress(SOCKADDR* sa, int salen);
};
