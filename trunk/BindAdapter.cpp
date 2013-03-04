#include "BindAdapter.h"
#include "main.h"
#include "cstdio"
#include "strsafe.h"
#include "string.h"
#include "DrvCall.h"
BindAdapter::BindAdapter(const char *vname,const char *name)
{
	vAdapter=vname;
	Adapter=name;
	type=0;
	InBound=OutBound=NULL;
	//其他的按初始化
}
/*
const string *  BindAdapter::getvName() const
{
	return &vAdapter;
}
const string *  BindAdapter::getName() const
{
	return &Adapter;
}
const string *  BindAdapter::getDesc() const
{
	return &Desc;
}
const string *  BindAdapter::getIp() const
{
	return &IP;
}
UINT  BindAdapter::getType()
{
	return type;
}

void BindAdapter::setvName(const char *s)
{
	vAdapter=s;

}
void BindAdapter::setName(const char *s)
{
	Adapter=s;
}
void BindAdapter::setIp(const char *s)
{
	IP=s;
}*/
void BindAdapter::setMac(const USHORT *s)
{
	char c[13]={0};
	memcpy(Mac,s,6);
	MactoStr(Mac,c);
	Mac_str=string(c);
}/*
void BindAdapter::setDesc(const char *s)
{
	Desc=s;
}
void BindAdapter::setType(UINT s)
{
	type=s;
}
BOOL BindAdapter::isSameMac(string s) const
{
	USHORT r[3];
	strMacConv(s,r);
	return isSameMac((USHORT *)r);
}*/
BOOL BindAdapter::isSameMac(USHORT * s) const
{
	int i=0;
	for(i=0;i<3;i++)
		if(s[i]!=Mac[i])break;
	return i==3;

}
/*
BOOL BindAdapter::isSameIP(string s) const
{
	return s==IP;
}*/
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
/*
const string  *BindAdapter::getMac() const
{
	
	return &Mac_str;
}

BindAdapter * BindAdapter::BeginRequest() const 
{
	return const_cast<BindAdapter *>(this);
}
*/
UINT BindAdapter::CloseInHandle()
{
	if(InBound==NULL)return 0;
	if(CloseHandle(InBound))
	{
	InBound=NULL;
	return 0;
	}
	else
	{
		return 0;
	}
}

UINT BindAdapter::CloseOutHandle()
{
	if(OutBound==NULL)return 0;
	if(CloseHandle(OutBound))
	{
	OutBound=NULL;
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
	if(InBound!=NULL)return 0;
	InBound=DrvCall::OpenLowerAdapter(Adapter.c_str());
	if(InBound==NULL||InBound==INVALID_HANDLE_VALUE)
	{
		InBound=NULL;
		return 1;
	}
	return 0;
}
UINT BindAdapter::OpenOutBound()
{
	if(OutBound!=NULL)return 0;
	OutBound=DrvCall::OpenVirtualAdapter(vAdapter.c_str());
	if(OutBound==NULL||OutBound==INVALID_HANDLE_VALUE)
	{
		OutBound=NULL;
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
	if(InBound==NULL)return 0;
	return DrvCall::ResetPktRedirFilter(InBound);
}
UINT BindAdapter::ResetOutHook() const
{
	if(OutBound==NULL)return 0;
	return DrvCall::ResetPktRedirFilter(OutBound);
}


UINT BindAdapter::SetInHook(PPKT_REDIR_FILTER_ENTRY pPktRedirFilterList, 
							ULONG nPktRedirFilterListEntryCount ) const{
	if(InBound==NULL)return 1;
	return DrvCall::SetPktRedirFilter(InBound,pPktRedirFilterList,nPktRedirFilterListEntryCount);
}
UINT BindAdapter::SetOutHook(PPKT_REDIR_FILTER_ENTRY pPktRedirFilterList, 
							 ULONG nPktRedirFilterListEntryCount ) const{

	if(OutBound==NULL)return 1;
	return DrvCall::SetPktRedirFilter(OutBound,pPktRedirFilterList,nPktRedirFilterListEntryCount);
}