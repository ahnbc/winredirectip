#ifndef _H_CLS_DRVCALL
#define  _H_CLS_DRVCALL
#include "main.h"
//#include <process.h>
#include "winsock.h"
#include <iphlpapi.h>
#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "ws2_32.lib")

typedef DWORD  
	(WINAPI* PCASIM_GetDriverCapability)( 
	HANDLE hHandle, 
	UINT * pRes);

typedef  DWORD  
	(WINAPI*   PCASIM_EnumerateBindings)( 
	HANDLE hWDMHandle, 
	int * pNdisStatus, 
	PWCHAR pBuffer, 
	PUINT pBufferSize // On entry this must point to size of buffer. 
	); 

typedef HANDLE ( WINAPI *
				PCASIM_OpenVirtualAdapter)( const TCHAR *pszAdapterName ); 

typedef HANDLE ( WINAPI*
				PCASIM_OpenLowerAdapter)(const TCHAR *pszAdapterName ); 

typedef DWORD (WINAPI *
			   PCASIM_GetAdapterVendorDescription)( 
			   HANDLE hAdapterHandle, 
			   int * pNdisStatus, 
			   TCHAR *lpBuffer, 
			   LPDWORD pBufferSize // Bytes at lpBuffer 
			   ); 

typedef DWORD ( WINAPI *
			   PCASIM_GetAdapterCurrentAddress)( 
			   HANDLE hAdapterHandle, 
			   int * pNdisStatus, 
			   void * pNdisMedium, // Optional. May be NULL. 
			   PUCHAR pAdapterAddress, 
			   PULONG pAddressLength 
			   ); 


typedef DWORD (WINAPI * 
			   PCASIM_SetPktRedirFilter)( 
			   HANDLE hAdapter, 
			   PPKT_REDIR_FILTER_ENTRY pPktRedirFilterList, 
			   ULONG nPktRedirFilterListEntryCount 
			   ); 

typedef DWORD ( WINAPI*
			   PCASIM_ResetPktRedirFilter)( 
			   HANDLE hAdapter 
			   ); 


class DrvCall
{
	private:
		static PCASIM_ResetPktRedirFilter pPCASIM_ResetPktRedirFilter;
		static PCASIM_GetAdapterCurrentAddress pPCASIM_GetAdapterCurrentAddress;
		static PCASIM_OpenLowerAdapter pPCASIM_OpenLowerAdapter;
		static PCASIM_OpenVirtualAdapter pPCASIM_OpenVirtualAdapter;
		static PCASIM_EnumerateBindings pPCASIM_EnumerateBindings;
		static PCASIM_GetDriverCapability pPCASIM_GetDriverCapability;
		static PCASIM_GetAdapterVendorDescription pPCASIM_GetAdapterVendorDescription;
		static PCASIM_SetPktRedirFilter pPCASIM_SetPktRedirFilter;
		static HANDLE mainIo;
		static HINSTANCE  lib;
	public:
		static DWORD SetPktRedirFilter(HANDLE hAdapter, 
			PPKT_REDIR_FILTER_ENTRY pPktRedirFilterList, 
			ULONG nPktRedirFilterListEntryCount );
		static DWORD ResetPktRedirFilter( 
			HANDLE hAdapter );
		static DWORD GetAdapterCurrentAddress( 
			HANDLE hAdapterHandle, 
			int * pNdisStatus, 
			void * pNdisMedium, // Optional. May be NULL. 
			PUCHAR pAdapterAddress, 
			PULONG pAddressLength 
			); 
		static DWORD  
			GetDriverCapability( 
			HANDLE hHandle, 
			UINT * pRes);

		static DWORD  
			EnumerateBindings( 
			int * pNdisStatus, 
			PWCHAR pBuffer, 
			PUINT pBufferSize // On entry this must point to size of buffer. 
			); 

		static HANDLE OpenVirtualAdapter(const TCHAR *pszAdapterName ); 

		static HANDLE 
			  OpenLowerAdapter(const TCHAR *pszAdapterName ); 

		static DWORD GetAdapterVendorDescription( 
			HANDLE hAdapterHandle, 
			int * pNdisStatus, 
			TCHAR *lpBuffer, 
			LPDWORD pBufferSize // Bytes at lpBuffer 
			); 
		static UINT OpenIo();
		static UINT OpenLib();
		static UINT Init();
		static UINT Free();
};


#endif