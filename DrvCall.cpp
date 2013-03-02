#include "DrvCall.h"
PCASIM_EnumerateBindings DrvCall::pPCASIM_EnumerateBindings=0;
PCASIM_GetAdapterVendorDescription	DrvCall::pPCASIM_GetAdapterVendorDescription=0;
PCASIM_GetAdapterCurrentAddress DrvCall::pPCASIM_GetAdapterCurrentAddress=0;
PCASIM_GetDriverCapability DrvCall::pPCASIM_GetDriverCapability=0;
PCASIM_SetPktRedirFilter DrvCall::pPCASIM_SetPktRedirFilter=0;
PCASIM_ResetPktRedirFilter DrvCall::pPCASIM_ResetPktRedirFilter=0;
PCASIM_OpenLowerAdapter DrvCall::pPCASIM_OpenLowerAdapter=0;
PCASIM_OpenVirtualAdapter DrvCall::pPCASIM_OpenVirtualAdapter=0;
HANDLE DrvCall::mainIo=NULL;
HINSTANCE  DrvCall::lib=NULL;
DWORD DrvCall::SetPktRedirFilter(HANDLE hAdapter, 
						   PPKT_REDIR_FILTER_ENTRY pPktRedirFilterList, 
						   ULONG nPktRedirFilterListEntryCount )
{
	return pPCASIM_SetPktRedirFilter(hAdapter, 
		pPktRedirFilterList, 
		nPktRedirFilterListEntryCount );
}
DWORD DrvCall::ResetPktRedirFilter( 
	HANDLE hAdapter )
{
	if(pPCASIM_ResetPktRedirFilter)
	return pPCASIM_ResetPktRedirFilter(hAdapter);
	return 1;
}
DWORD DrvCall::GetAdapterCurrentAddress( 
	HANDLE hAdapterHandle, 
	int * pNdisStatus, 
	void * pNdisMedium, // Optional. May be NULL. 
	PUCHAR pAdapterAddress, 
	PULONG pAddressLength 
	)
{
	if(pPCASIM_GetAdapterCurrentAddress)
	return pPCASIM_GetAdapterCurrentAddress(hAdapterHandle,pNdisStatus, \
		pNdisMedium,pAdapterAddress,pAddressLength);
	return 1;
}
DWORD DrvCall::GetDriverCapability( 
	HANDLE hHandle, 
	UINT * pRes)
{
	if(pPCASIM_GetDriverCapability)
	return pPCASIM_GetDriverCapability(hHandle,pRes);
	return 1;
}

DWORD DrvCall::EnumerateBindings( 
	int * pNdisStatus, 
	PWCHAR pBuffer, 
	PUINT pBufferSize // On entry this must point to size of buffer. 
	)
{
	if(pPCASIM_EnumerateBindings&&mainIo)
	return pPCASIM_EnumerateBindings(mainIo,pNdisStatus,pBuffer,pBufferSize);
	return 1;
}

HANDLE DrvCall::OpenVirtualAdapter(const TCHAR *pszAdapterName )
{
	if(pPCASIM_OpenVirtualAdapter)
	return pPCASIM_OpenVirtualAdapter(pszAdapterName);
	return INVALID_HANDLE_VALUE;
}
HANDLE DrvCall::OpenLowerAdapter(const TCHAR *pszAdapterName )
{
	if(pPCASIM_OpenLowerAdapter)
	return pPCASIM_OpenLowerAdapter(pszAdapterName);
	return INVALID_HANDLE_VALUE;
}

DWORD DrvCall::GetAdapterVendorDescription( 
	HANDLE hAdapterHandle, 
	int * pNdisStatus, 
	TCHAR *lpBuffer, 
	LPDWORD pBufferSize // Bytes at lpBuffer 
	){

		if(pPCASIM_GetAdapterVendorDescription)
		return pPCASIM_GetAdapterVendorDescription(hAdapterHandle,pNdisStatus,lpBuffer,pBufferSize);
		return 1;
}
UINT DrvCall::OpenIo()
{
	if(mainIo!=NULL)return 0;
	mainIo=CreateFile(IOName,0xC0000000,0,0,2,0x80,0);
	if(mainIo==(HANDLE)INVALID_HANDLE_VALUE)
	{
		mainIo=0;
		//err 3 无法读入驱动
		return 3;
	}
	return 0;
}
UINT DrvCall::OpenLib(){
	if(lib==NULL)
		lib=LoadLibrary("PcaFilterApi.dll");
	if(lib==NULL)
	{
		//err 1 库不存在
		return 1;
	}
	if(!pPCASIM_ResetPktRedirFilter)
		pPCASIM_ResetPktRedirFilter=(PCASIM_ResetPktRedirFilter)GetProcAddress(lib,"PCASIM_ResetPktRedirFilter");
	if(!pPCASIM_SetPktRedirFilter)
		pPCASIM_SetPktRedirFilter=(PCASIM_SetPktRedirFilter)GetProcAddress(lib,"PCASIM_SetPktRedirFilter");
	if(!pPCASIM_GetAdapterCurrentAddress)
		pPCASIM_GetAdapterCurrentAddress=(PCASIM_GetAdapterCurrentAddress)GetProcAddress(lib,"PCASIM_GetAdapterCurrentAddress");
	if(!pPCASIM_OpenVirtualAdapter)
		pPCASIM_OpenVirtualAdapter=(PCASIM_OpenVirtualAdapter)GetProcAddress(lib,"PCASIM_OpenVirtualAdapterA");
	if(!pPCASIM_EnumerateBindings)
		pPCASIM_EnumerateBindings=(PCASIM_EnumerateBindings)GetProcAddress(lib,"PCASIM_EnumerateBindings");
	if(!pPCASIM_OpenLowerAdapter)
		pPCASIM_OpenLowerAdapter=(PCASIM_OpenLowerAdapter)GetProcAddress(lib,"PCASIM_OpenLowerAdapterA");
	if(!pPCASIM_GetAdapterVendorDescription)
		pPCASIM_GetAdapterVendorDescription=(PCASIM_GetAdapterVendorDescription)GetProcAddress(lib,"PCASIM_GetAdapterVendorDescriptionA");
	if(!pPCASIM_GetDriverCapability)
		pPCASIM_GetDriverCapability=(PCASIM_GetDriverCapability)GetProcAddress(lib,"PCASIM_GetDriverCapability");
	if((!pPCASIM_ResetPktRedirFilter)||
		(!pPCASIM_SetPktRedirFilter)||
		(!pPCASIM_GetAdapterCurrentAddress)||
		(!pPCASIM_OpenVirtualAdapter)||
		(!pPCASIM_EnumerateBindings)||
		(!pPCASIM_OpenLowerAdapter)||
		(!pPCASIM_GetAdapterVendorDescription)||
		(!pPCASIM_GetDriverCapability)
		)
		//err 2 库函数无法读入
		return 2;

	return 0;
}
UINT DrvCall::Init(){
	UINT ret=0;
	if((ret=OpenLib())||(ret=OpenIo()))
		return ret;
	return 0;
}
UINT DrvCall::Free()
{
	if(mainIo!=NULL)
		CloseHandle(mainIo);
	if(lib!=NULL)
	{
		FreeLibrary(lib);
		lib=0;
	}
	return 0;
}