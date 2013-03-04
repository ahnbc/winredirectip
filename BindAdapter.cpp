#include "BindAdapter.h"
#include "main.h"
#include "cstdio"
#include "strsafe.h"
#include "string.h"
#include "DrvCall.h"
BindAdapter::BindAdapter(const char *vname,const char *name)
{
	m_svAdapter=vname;
	m_sAdapter=name;
	m_nType=0;
	m_hInBound=m_hOutBound=NULL;
	//其他的按初始化
}

void BindAdapter::setMac(const USHORT *s)
{
	char c[13]={0};
	memcpy(m_Mac,s,6);
	MactoStr(m_Mac,c);
	m_sMac=string(c);
}
BOOL BindAdapter::isSameMac(USHORT * s) const
{
	int i=0;
	for(i=0;i<3;i++)
		if(s[i]!=m_Mac[i])break;
	return i==3;

}

UINT BindAdapter::strMacConv(string s,USHORT *m) 
{
	char c[3]={0};
	int i=0;
	uchar *mac=(uchar *)m;
	for(i=0;i<6;i++)
	{
		memcpy(c,s.c_str()+i*2,2);
#define A(x) (toupper(x)>='A'?(toupper(x)-'A'+10):(x-'0'))
		mac[i]=(uchar)(A(c[1])*16+A(c[2]));
#undef A
	}
	return 0;
}
UINT BindAdapter::MactoStr(USHORT * m ,char *s) 
{
	uchar *mac=(uchar *)m;
	int i=0;
	for(i=0;i<6;i++)
	{
	//	StringCchPrintfA(s+2*i,3,"%2.2X",mac[i]);
		StringCchPrintfA(s+2*i,3,"%2.2X",mac[i]);
	}
	s[12]=0;
	return 0;
}

UINT BindAdapter::CloseInHandle()
{
	if(m_hInBound==NULL)return 0;
	if(CloseHandle(m_hInBound))
	{
	m_hInBound=NULL;
	return 0;
	}
	else
	{
		return 0;
	}
}

UINT BindAdapter::CloseOutHandle()
{
	if(m_hOutBound==NULL)return 0;
	if(CloseHandle(m_hOutBound))
	{
	m_hOutBound=NULL;
	return 0;
	}
	else
	{
		return 1;
	}
}


UINT BindAdapter::CloseHandles()
{
	UINT ret=0;
	if((ret=CloseOutHandle())||(ret=CloseInHandle()))
		return ret;
	return 0;
}

UINT BindAdapter::OpenHandles()
{
	UINT ret=0;
	if((ret=OpenInBound())||(ret=OpenOutBound()))
		return ret;
	return 0;
}
UINT BindAdapter::OpenInBound()
{
	if(m_hInBound!=NULL)return 0;
	m_hInBound=DrvCall::OpenLowerAdapter(m_sAdapter.c_str());
	if(m_hInBound==NULL||m_hInBound==INVALID_HANDLE_VALUE)
	{
		m_hInBound=NULL;
		return 1;
	}
	return 0;
}
UINT BindAdapter::OpenOutBound()
{
	if(m_hOutBound!=NULL)return 0;
	m_hOutBound=DrvCall::OpenVirtualAdapter(m_svAdapter.c_str());
	if(m_hOutBound==NULL||m_hOutBound==INVALID_HANDLE_VALUE)
	{
		m_hOutBound=NULL;
		return 1;
	}
	return 0;
}

UINT BindAdapter::ResetHook() const
{
	UINT ret=0;
	if((ret=ResetInHook())||(ret=ResetOutHook()))
		return ret;
	return 0;
	
}
UINT BindAdapter::ResetInHook() const
{
	if(m_hInBound==NULL)return 0;
	return DrvCall::ResetPktRedirFilter(m_hInBound);
}
UINT BindAdapter::ResetOutHook() const
{
	if(m_hOutBound==NULL)return 0;
	return DrvCall::ResetPktRedirFilter(m_hOutBound);
}


UINT BindAdapter::SetInHook(PPKT_REDIR_FILTER_ENTRY pPktRedirFilterList, 
							ULONG nPktRedirFilterListEntryCount ) const{
	if(m_hInBound==NULL)return 1;
	return DrvCall::SetPktRedirFilter(m_hInBound,pPktRedirFilterList,nPktRedirFilterListEntryCount);
}
UINT BindAdapter::SetOutHook(PPKT_REDIR_FILTER_ENTRY pPktRedirFilterList, 
							 ULONG nPktRedirFilterListEntryCount ) const{

	if(m_hOutBound==NULL)return 1;
	return DrvCall::SetPktRedirFilter(m_hOutBound,pPktRedirFilterList,nPktRedirFilterListEntryCount);
}