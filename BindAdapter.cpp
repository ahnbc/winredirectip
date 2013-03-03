#include "BindAdapter.h"
#include "main.h"
#include "cstdio"
#include "strsafe.h"
BindAdapter::BindAdapter(const char *vname,const char *name)
{
	vAdapter=vname;
	Adapter=name;
	type=0;
	InBound=OutBound=NULL;
	//其他的按初始化
}
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
}
void BindAdapter::setMac(const USHORT *s)
{
	char c[13]={0};
	memcpy(Mac,s,6);
	MactoStr(Mac,c);
	Mac_str=string(c);
}
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
}
BOOL BindAdapter::isSameMac(USHORT * s) const
{
	int i=0;
	for(i=0;i<3;i++)
		if(s[i]!=Mac[i])break;
	return i==3;

}
BOOL BindAdapter::isSameIP(string s) const
{
	return s==IP;
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
		StringCchPrintfA(s+2*i,3,"%2.2X",mac[i]);
	}
	s[12]=0;
	return 0;
}
const string  *BindAdapter::getMac() const
{
	
	return &Mac_str;
}


UINT BindAdapter::CloseInHandle() const
{
	BindAdapter * use_in_care=const_cast<BindAdapter *>(this);
	if(InBound==0)return 0;
	CloseHandle(InBound);
	use_in_care->InBound=0;
	return 0;
}

UINT BindAdapter::CloseOutHandle() const
{
	BindAdapter * use_in_care=const_cast<BindAdapter *>(this);
	if(OutBound==0)return 0;
	CloseHandle(OutBound);
	use_in_care->OutBound=0;
	return 0;
}


UINT BindAdapter::CloseHandles() const
{
	UINT ret=0;
	if((ret=CloseOutHandle())||(ret=CloseInHandle()))
		return ret;
	return 0;
}