#ifndef _H_MSXDEX
#define _H_MSXDEX
#define _CRT_SECURE_NO_WARNINGS
#define WINVER 0x0502
#define _WIN32_WINNT WINVER
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
		const wchar_t IOName[]=L"\\\\.\\PcaIpRedir";
 typedef void (_stdcall *MyCallBack) (byte *,DWORD);		
 UINT  WINAPI Free(const byte);
 UINT WINAPI  redirIP(const wchar_t [],const wchar_t[],const wchar_t[],const UCHAR ,const USHORT );
 void WINAPI DllInit();
 void WINAPI RegisterNoPort(const USHORT *,DWORD);
 void WINAPI RegisterInCallBack(const MyCallBack);
 void WINAPI RegisterOutCallBack(const MyCallBack);
#ifdef __cplusplus
}
#endif

#endif