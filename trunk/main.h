/************************************************************************/
/**
 * @file: main
 * @project: winipredir
 * @author:nowind
 * @email:nowind01@gmail.com
 * @date:2013/03/27
 * @version: 0.1.2
 * @description: 利用 NDIS 转发 ip包
 *   只允许系统中转发一个ip 
 *   
 * 
 * 
/************************************************************************/


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
#ifndef IN
#define IN
#endif
#ifndef OUT
#define OUT
#endif
#ifndef INOUT
#define INOUT
#endif
#define PASS 0
		#define  BLOCK 1
		#define  READ 2
		#define  REDIRECT 3
		const wchar_t IOName[]=L"\\\\.\\PcaIpRedir";
 typedef void (_stdcall *MyCallBack) (IN byte,///< 协议
	 INOUT byte *,///< 应用层数据
	 IN DWORD ///< 长度
	 );		
 UINT  WINAPI Free(IN const byte///<释放标志 1 强制全部释放 0 
	 );
 UINT WINAPI  redirIP(IN const wchar_t [], ///< 设备描述
	 IN const wchar_t[], ///< 要转发的ip
	 IN const wchar_t[],
	 IN const UCHAR ,
	 IN const USHORT );
 void WINAPI DllInit();
 void WINAPI RegisterNoPort(IN const USHORT *,
	 IN DWORD);
 void WINAPI RegisterInCallBack(IN const MyCallBack);
 void WINAPI RegisterOutCallBack(IN const MyCallBack);
#ifdef __cplusplus
}
#endif

#endif