#ifndef _H_MSXDEX
#define _H_MSXDEX
#define WINVER 0x0502
#define _WIN32_WINNT 0x0502
#include "windows.h"
#ifndef __MINGW32__
//#include "ntddndis.h"
#else
//#include "ddk/ntddndis.h"
#endif
#define EX_API __declspec(dllexport)
#ifndef uint
#define uint DWORD
#endif
#ifndef ushort
#define ushort unsigned short
#endif
#ifndef uchar
#define uchar unsigned char
#endif
#ifdef __cplusplus
extern "C"{
#endif
//typedef  uint NDIS_STATUS,NDIS_MEDIUM;
//typedef  uint *PNDIS_STATUS,*PNDIS_MEDIUM;
typedef 
struct _PKT_REDIR_FILTER_ENTRY 
{ 
ULONG m_IPSrcAddressRangeStart; // Start Of Inclusive Range 
ULONG m_IPSrcAddressRangeEnd; // End Of Inclusive Range 
ULONG m_IPDstAddressRangeStart; // Start Of Inclusive Range 
ULONG m_IPDstAddressRangeEnd; // End Of Inclusive Range 
ULONG m_nFilterAction; // Action Bitmap. 
ULONG m_nReserved; 
} 
PKT_REDIR_FILTER_ENTRY, *PPKT_REDIR_FILTER_ENTRY;
typedef unsigned short u16;
typedef unsigned long u32;
#define PASS 0
		#define  BLOCK 1
		#define  READ 2
		#define  REDIRECT 3
		const char IOName[]="\\\\.\\PcaIpRedir";
		
 void  WINAPI Free();
 UINT WINAPI  redirIP(const char [],const char[],const char[],UCHAR proto,USHORT fport);
 UINT WINAPI ListRedirSupportAdptersMac(uchar*,UINT *);
 UINT WINAPI GetvAdpByMac(char *mac,char *buffer,uint *size);
 UINT WINAPI GetlAdpByMac(char *mac,char *buffer,uint *size);
 
#ifdef __cplusplus
}
#endif

#endif