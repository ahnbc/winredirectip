#include "BindList.h"
#include "DrvCall.h"
#include "string.h"
BindList BindList::_self=BindList();
uint BindList::err=0;
 BindList * BindList::getAllBindList()
{
	uint ret,i,j;
	int status;
	UINT size;
	WCHAR *rawbuff;
	char *buff;
	HANDLE hAdp;
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter = NULL;
	DWORD dwRetVal = 0;
	ULONG ulOutBufLen = sizeof (IP_ADAPTER_INFO);
	USHORT mac[3];
	vector<BindAdapter>::iterator it;

	//--------------------------------------------
	if(BindList::_self.Adapters.size()>0)
	{
		BindList::err=0;
		return &BindList::_self;
	}
	ret=DrvCall::Init();

	if(ret)
	{
		BindList::err=ret;
		return NULL;
	}
	status=0;
	size=0;
	//开始获取两个ID
	if(DrvCall::EnumerateBindings(&status,NULL,&size)!=0)
	{
		//err 4 无法查询绑定接口
		BindList::err=4;
		return NULL;
	}
	if(status!=0&&status!=(int)0xC0010016)
	{
		//err 5 结果状态错误
		BindList::err=5;
		return NULL;
	}
	rawbuff=(WCHAR *)malloc(size);
	
	if(DrvCall::EnumerateBindings(&status,rawbuff,&size)!=0)
	{
		//err 6 获取buff错误
		free(rawbuff);
		BindList::err=6;
		return NULL;
	}
	if(status!=0)
	{
		//err 7 结果状态错误
		BindList::err=7;
		free(rawbuff);
		return NULL;
	}
	buff=(CHAR *)malloc(size/2+1);
	WideCharToMultiByte(CP_ACP,0,rawbuff,size/2,buff,size/2+1,NULL,&status);
	if(status!=0)
	{
		//err 8 字符转码错误
		//	printf("Char Error\n");
			free(buff);
		free(rawbuff);
		BindList::err=8;
		return NULL;
	}
	for(i=0;i<size/2-1;i++)
	{
		j=(DWORD)strlen(buff+i);
		BindList::_self.Adapters.push_back(BindAdapter(buff+i,buff+i+j+1));
		i+=j+1;
		j=(DWORD)strlen(buff+i);
		i+=j;
	}

	//获取mac地址
	for(it=BindList::_self.Adapters.begin();it!=BindList::_self.Adapters.end();it++)
	{
		hAdp=DrvCall::OpenLowerAdapter(it->getName()->c_str());
		if(hAdp==INVALID_HANDLE_VALUE)
					//err 15 ??
				{
					free(buff);
					free(rawbuff);
					BindList::err=15;
					return NULL;
				}
		status=0;
		ret=20;
		if(DrvCall::GetAdapterCurrentAddress(hAdp,&status,NULL,(PUCHAR)rawbuff,&ret))
			{
				free(buff);
				free(rawbuff);
						//err 18
				BindList::err=18;
				return NULL;
			};
		if(status!=0)
			{
				free(buff);
				free(rawbuff);
						//err 16 ??
				BindList::err=16;
				return NULL;
			}
		if(ret>0)
			{
				it->setMac((USHORT *) rawbuff);
			}
			
	}
	//
	free(buff);
	free(rawbuff);

	//开始获取IP信息
	pAdapterInfo = (IP_ADAPTER_INFO *) malloc(sizeof (IP_ADAPTER_INFO));
	if (pAdapterInfo == NULL) {
		//err 13 内存申请不足
		//	printf("Error allocating memory needed to call GetAdaptersinfo\n");
		BindList::err=13;
		return NULL;
	}
	// Make an initial call to GetAdaptersInfo to get
	// the necessary size into the ulOutBufLen variable
	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
		free(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *) malloc(ulOutBufLen);
		if (pAdapterInfo == NULL) {
			//printf("Error allocating memory needed to call GetAdaptersinfo\n");
			BindList::err=13;
			return NULL;
		}
	}

	if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
		pAdapter = pAdapterInfo;
		//printf("Adapter List: \n");
		for (;pAdapter!=NULL;pAdapter = pAdapter->Next) {
			if(pAdapter->AddressLength!=6)continue;
			memcpy(mac,pAdapter->Address,6);
			for(it=BindList::_self.Adapters.begin();it!=BindList::_self.Adapters.end();it++)
			{
				if(it->isSameMac(mac))
				{
					it->setIp(pAdapter->IpAddressList.IpAddress.String);
					it->setDesc(pAdapter->Description);
					it->setType(pAdapter->Type);
				}
			}
		}
	} else {
		BindList::err=19;
		return NULL;
		//printf("GetAdaptersInfo failed with error: %d\n", dwRetVal);
	}
	if (pAdapterInfo)
		free(pAdapterInfo);
	return &BindList::_self;
}

uint BindList::GetError()
{
	return BindList::err;
}
const BindAdapter * BindList::getByMac(string mac) const
{
	vector<BindAdapter>::const_iterator it;
	for(it=Adapters.begin();it!=Adapters.end();it++)
		if(it->isSameMac(mac))return &(*it);
	return NULL;
}
const BindAdapter * BindList::getByIP(string ip)  const
{
	vector<BindAdapter>::const_iterator it;
	for(it=Adapters.begin();it!=Adapters.end();it++)
		if(it->isSameIP(ip))return &(*it);
	return NULL;
}
const BindAdapter * BindList::getByName(string s)  const
{
	vector<BindAdapter>::const_iterator it;
	for(it=Adapters.begin();it!=Adapters.end();it++)
		if((*it->getName())==s)return &(*it);
	return NULL;
}
const vector<BindAdapter> *BindList::getLists() const
{
	return &Adapters;
}
uint BindList::getSize()
{
	return (uint)Adapters.size();
}