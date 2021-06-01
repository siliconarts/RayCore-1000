#include "NetworkAdapter.h"

NetworkAdapter	g_NetworkAdapter;

NetworkAdapter::NetworkAdapter(void){
	m_pAdapterInfo	= NULL;
	m_pBuffer		= NULL;
	{
		ULONG OutBufLen = 0;
		if( GetAdaptersInfo(NULL, &OutBufLen) == ERROR_BUFFER_OVERFLOW ){
			m_pBuffer		= new BYTE[OutBufLen];
			m_pAdapterInfo	= (PIP_ADAPTER_INFO)m_pBuffer;

			//Get info for the adapters
			if( GetAdaptersInfo(m_pAdapterInfo, &OutBufLen) != ERROR_SUCCESS )
			{
				//Call failed
				delete[] m_pBuffer;
				m_pBuffer	= NULL;
			}
		}
	}
}

NetworkAdapter::~NetworkAdapter(void){
	if(m_pBuffer){
		delete[] m_pBuffer;
		m_pBuffer = NULL;
	}
	m_pAdapterInfo	= NULL;
}

PIP_ADAPTER_INFO NetworkAdapter::GetAdapters(void){
	return m_pAdapterInfo;
}
