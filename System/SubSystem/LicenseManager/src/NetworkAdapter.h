#ifndef __NETWORK_ADAPTER_H__
#define __NETWORK_ADAPTER_H__
#include <winsock2.h>
#include <windows.h>
#pragma comment(lib,"Iphlpapi.lib")
#include <Iphlpapi.h>

/*
#pragma comment(lib,"Rpcrt4.lib")
#include <netcon.h>*/


class NetworkAdapter
{
public:
	NetworkAdapter(void);
	~NetworkAdapter(void);
private:
	PIP_ADAPTER_INFO	m_pAdapterInfo;
	int					m_iCount;
	BYTE*				m_pBuffer;
public:
	bool IsValid(void)			{return m_pAdapterInfo != NULL;}
	inline int GetCount(void)	{return m_iCount;}
	PIP_ADAPTER_INFO GetAdapters(void);
};

extern NetworkAdapter	g_NetworkAdapter;

#endif//__NETWORK_ADAPTER_H__
