
#include "main.h"
#include "cstdio"
#include "cstdlib"
#include <process.h>
#include "winsock.h"
#include <string.h>
#include <iphlpapi.h>
#include "DrvCall.h"
#include "BindAdapter.h"
#include "BindList.h"
#include "vector"
#include "getopt.h"
#include "wchar.h"
#include "signal.h"
#include "AutoBuffer.h"
#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "ws2_32.lib")


/*
static BOOL WINAPI 
PCASIM_MakeNdisRequest( 
HANDLE hDevice, 
PSIM_REQUEST pSIMRequest, 
LPOVERLAPPED lpOverlapped 
); 
*/
#define MAXFF 0xFFFFFFFF

/// 全局变量 空间
namespace global
{
const CBindAdapter *g_cpAdp=NULL;
static HANDLE g_hInThread=NULL;
static HANDLE g_hOutThread=NULL;
static HANDLE g_hMainThread=NULL;
static BYTE   g_ulMACAddr[8]={0};
static BOOL   g_bMacLocal=false;
static  uint g_dworgIP=0;//={221,231,130,70};
static USHORT g_port=0;
static UCHAR g_protocol=0;
static uint g_dwrediIP=0;
static CRITICAL_SECTION cs;
static PKT_REDIR_FILTER_ENTRY g_Outent={0,MAXFF,0,MAXFF,REDIRECT,0};
static PKT_REDIR_FILTER_ENTRY g_Inent={0,MAXFF,0,MAXFF,REDIRECT,0};
static vector<USHORT> noredirport;
static MyCallBack g_InCallBack,g_OutCallBack;
};
/// 常量空间
namespace const_var
{
	enum
	{
	IN_DIRECT=1,
	OUT_DIRECT=2
	};
const static USHORT IP_PRO=0x0008,
	PPPOE_PRO=0x6488,PPPOE_IP_PRO=0x2100,
	LEN_ENT=14,LEN_PPPOE=8;
const static UCHAR TCP_PRO=6,UDP_PRO=17;
};

/// 自定义类型
typedef struct
{
	unsigned long saddr; //源IP地址      
	unsigned long daddr; //目的IP地址       
	uchar mbz;                   // mbz = must be zero, 用于填充对齐
	uchar protocal;             //8位协议号       
	unsigned short tcpl;    // TCP包长度      
}psdheader_t;



/// 回调函数
 VOID CALLBACK InIOReadCompletionRoutine(   
                  DWORD dwErrorCode,   
                  DWORD dwNumberOfBytesTransfered,   
                  LPOVERLAPPED lpOverlapped );
 VOID CALLBACK OutIOReadCompletionRoutine(   
                  DWORD dwErrorCode,   
                  DWORD dwNumberOfBytesTransfered,   
                  LPOVERLAPPED lpOverlapped );
  VOID CALLBACK InIOWriteCompletionRoutine(   
                  DWORD dwErrorCode,   
                  DWORD dwNumberOfBytesTransfered,   
                  LPOVERLAPPED lpOverlapped );
  VOID CALLBACK OutIOWriteCompletionRoutine(   
                  DWORD dwErrorCode,   
                  DWORD dwNumberOfBytesTransfered,   
                  LPOVERLAPPED lpOverlapped );

  /// 函数声明
  /// 计算ip头的校验字
  USHORT ip_checksum(IN USHORT* buffer, 
					IN int size);
  /// tcp udp头的计算
  u16 tcpudp_sum_calc(IN byte protocal,
	  IN u16 len_tcp,
	  IN u16 src_addr[],
	  IN u16 dest_addr[], 
	  IN u16 buff[] ///< 第四层开始的buff
  );
  //判断当前是否是64位系统运行32位程序
  BOOL IsWow64Current();

  void AtExit(void);
  void signalExit(int);

   ///  写数据到网卡的完成io回调函数
   void IOWriteCompletionRoutine( 
	   IN byte byDirect, ///< 方向
	   IN DWORD dwErrorCode,  ///< 以下标准回调 
	   IN DWORD dwNumberOfBytesTransfered,   
	   IN LPOVERLAPPED lpOverlapped );
   VOID CALLBACK InIOWriteCompletionRoutine(   
	   DWORD dwErrorCode,   
	   DWORD dwNumberOfBytesTransfered,   
	   LPOVERLAPPED lpOverlapped );
   VOID CALLBACK OutIOWriteCompletionRoutine(   
	   DWORD dwErrorCode,   
	   DWORD dwNumberOfBytesTransfered,   
	   LPOVERLAPPED lpOverlapped );

   void  IOReadCompletionRoutine(   
	   byte byDirect,
	   DWORD dwErrorCode,   
	   DWORD dwNumberOfBytesTransfered,   
	   LPOVERLAPPED lpOverlapped );
   VOID CALLBACK InIOReadCompletionRoutine(   
	   DWORD dwErrorCode,   
	   DWORD dwNumberOfBytesTransfered,   
	   LPOVERLAPPED lpOverlapped );
   VOID CALLBACK OutIOReadCompletionRoutine(   
	   DWORD dwErrorCode,   
	   DWORD dwNumberOfBytesTransfered,   
	   LPOVERLAPPED lpOverlapped );
   /// 工作线程的调度函数
   unsigned WorkFunc(IN byte byDirect);
   static unsigned __stdcall InWork(void * pList);
   static unsigned __stdcall OutWork(void * pList);
   /// 主调度线程 由他调用出入两个工作线程 并等待返回
   static unsigned __stdcall MainWork(void * pList);


  //---------------------------------------------以下为函数实现
   /// 没啥好说,参看rfc
USHORT ip_checksum(USHORT* buffer, int size) 
{
DWORD cksum = 0;

// Sum all the words together, adding the final byte if size is odd

while (size > 1) {
cksum += (unsigned long)(*buffer++);
size -= sizeof(USHORT);
}
if (size) {
cksum += *(UCHAR*)buffer;
}

// Do a little shuffling

cksum = (cksum >> 16) + (cksum & 0xffff);
cksum += (cksum >> 16);

// Return the bitwise complement of the resulting mishmash

return (USHORT)(~cksum);
}


u16 tcpudp_sum_calc(byte protocal,u16 len_tcp, u16 src_addr[],u16 dest_addr[],  u16 buff[])
{
u16 prot_tcp=((u16)protocal)<<8;
u32 sum=0;	
u16 i=len_tcp;	
	// Find out if the length of data is even or odd number. If odd,
	// add a padding byte = 0 at the end of packet
	//initialize sum to zero

while (i > 1) {
	sum += (unsigned long)(*buff++);
	i -= sizeof(USHORT);
}
if (i) {
	sum += *(UCHAR*)buff;
}
	

	// make 16 bit words out of every two adjacent 8 bit words and 
	// calculate the sum of all 16 vit words
	// add the TCP pseudo header which contains:
	// the IP source and destinationn addresses,
	
	// the protocol number and the length of the TCP packet
	// 
   sum+=(unsigned long)src_addr[0];
   sum+=(unsigned long)src_addr[1];
   sum+=(unsigned long)dest_addr[0];
   sum+=(unsigned long)dest_addr[1];

   sum = sum + prot_tcp + htons(len_tcp);

	// keep only the last 16 bits of the 32 bit calculated sum and add the carries
    //	while (sum>>16)
	//	sum = (sum & 0xFFFF)+(sum >> 16);
	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	// Take the one's complement of sum
return (USHORT)(~sum);
}

/// 原理是判断 IsWow64Process 函数是否被加载
BOOL IsWow64Current()  
{  
	FARPROC fnIsWow64Process;  
	BOOL bIsWow64;  


	bIsWow64 = FALSE;  
	fnIsWow64Process = GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "IsWow64Process");  


	if(fnIsWow64Process)  
		if(((BOOL (WINAPI *)(HANDLE, PBOOL))fnIsWow64Process)(GetCurrentProcess(), &bIsWow64))  
			return bIsWow64;  
	return FALSE;  
}


void AtExit(void)
{
	Free(1);
}
void signalExit(int)
{
	Free(1);
}
 UINT  WINAPI Free(const byte freelib)
{
	DWORD code;
	static int ret=0;
	using namespace global;
	DeleteCriticalSection(&global::cs);
	ret=DrvCall::Init();
	if(ret)
		return ret;
	if(g_cpAdp!=NULL){
		g_cpAdp->ResetHook();
		g_cpAdp->CloseHandles();
		g_cpAdp=NULL;
	}
	// 获取线程状态 存活的话要么强杀要么等待
	if(g_hInThread!=NULL)
	{
		ret=GetExitCodeThread(g_hInThread,&code);
		if(ret==STILL_ACTIVE)
		{
			if(freelib)
				TerminateThread(g_hInThread,1);
			WaitForSingleObject(g_hInThread,INFINITE);
		}
		CloseHandle(g_hInThread);
		g_hInThread=NULL;
	}
	if(g_hOutThread!=NULL)
	{
		ret=GetExitCodeThread(g_hOutThread,&code);
		if(ret==STILL_ACTIVE)
		{
			if(freelib)
				TerminateThread(g_hOutThread,1);
			WaitForSingleObject(g_hOutThread,INFINITE);

		}
		CloseHandle(g_hOutThread);
		g_hOutThread=NULL;
	}
	if(g_hMainThread!=NULL)
		{
		ret=GetExitCodeThread(g_hMainThread,&code);
		if(ret==STILL_ACTIVE)
		{
			if(freelib)
				TerminateThread(g_hMainThread,1);
			WaitForSingleObject(g_hMainThread,INFINITE);
		
		}
			CloseHandle(g_hMainThread);
			g_hMainThread=NULL;
		}
	if(freelib)
		DrvCall::Free();//may be not needed
	return 0;
}

 
 void IOWriteCompletionRoutine( 
	 byte byDirect,
	 DWORD dwErrorCode,   
	 DWORD dwNumberOfBytesTransfered,   
	 LPOVERLAPPED lpOverlapped )
 {
	 byte *buff=(byte *)lpOverlapped+sizeof(OVERLAPPED);
	 UINT ret;
	 if(dwErrorCode!=0)return;
	 ret=(byDirect==const_var::IN_DIRECT? \
		 global::g_cpAdp->ReadInEx(buff,0x1000,lpOverlapped,InIOReadCompletionRoutine): \
		 global::g_cpAdp->ReadOutEx(buff,0x1000,lpOverlapped,OutIOReadCompletionRoutine)
		 );
	 if(ret==0)
	 {
		 EnterCriticalSection(&global::cs);
		 wprintf(L"%lsIOWriteCompletionRoutine Last Error:%d\n",
			 byDirect==const_var::IN_DIRECT?L"In":L"Out",
			 GetLastError());
		 LeaveCriticalSection(&global::cs);
	 }
	 //IO Finish
 }
 VOID CALLBACK InIOWriteCompletionRoutine(   
                  DWORD dwErrorCode,   
                  DWORD dwNumberOfBytesTransfered,   
                  LPOVERLAPPED lpOverlapped )
 {
	
	 IOWriteCompletionRoutine( const_var::IN_DIRECT,dwErrorCode,   
		 dwNumberOfBytesTransfered,   
		 lpOverlapped );
	 
 }
VOID CALLBACK OutIOWriteCompletionRoutine(   
                  DWORD dwErrorCode,   
                  DWORD dwNumberOfBytesTransfered,   
                  LPOVERLAPPED lpOverlapped )
 {

	 IOWriteCompletionRoutine(const_var::OUT_DIRECT,dwErrorCode,   
		 dwNumberOfBytesTransfered,   
		 lpOverlapped );
	 
 }


/**
 * @function: IOReadCompletionRoutine
 * @parameters:
 *	- byDirect
 *		方向
 *	- dwNumberOfBytesTransfered
 *		字节数
 *	- lpOverlapped
 *		包括数据的一个指向重叠io相关的数据
 *	@description:
 *		解析从数据链路层开始的数据包 支持pppoe 和 以太网 解析
 *		修改ip并调用数据修改回调函数 最后重新计算校验字
 *
 **/
void  IOReadCompletionRoutine(   
	IN byte byDirect,
	IN DWORD dwErrorCode,   
	IN DWORD dwNumberOfBytesTransfered,   
	IN LPOVERLAPPED lpOverlapped )
{
	ushort sum=0,//校验字
		//以下为各种协议解析使用
		proto_3=0,ppp_next_pro=0,
		ip_offset=0,tcp_offset=0,tcp_all_len=0,ip_all_len,check_port=0,tcpudp_header_len=0;
	//校正到数据部分
	byte *buff=(byte *)lpOverlapped+sizeof(OVERLAPPED);
	byte proto_4=0;
	UCHAR ip_len;
	UINT ret;
	vector<USHORT>::iterator it;
	if(dwErrorCode!=0)return;
	if(dwNumberOfBytesTransfered>=(14+20))
	{
#if defined(DEBUG) || defined(_DEBUG)
		EnterCriticalSection(&global::cs);
		printf("Read One Packet...\n");
    	LeaveCriticalSection(&global::cs);
#endif
		// 详见以太协议包
		proto_3=*(USHORT * )(buff+12);
		if(proto_3==const_var::IP_PRO)
			// 直接ip协议的话ip头偏移为以太包长度
			ip_offset=const_var::LEN_ENT;
		// pppoe协议
		else if(proto_3==const_var::PPPOE_PRO)
		{
			// pppoe下层协议为 ip的话
			ppp_next_pro=*(USHORT *)(buff+const_var::LEN_ENT+6);
			if(ppp_next_pro==const_var::PPPOE_IP_PRO)
				ip_offset=const_var::LEN_ENT+const_var::LEN_PPPOE;
			else goto __write;
		}
		// 不是ip协议直接写
		else 	goto __write;
		// ip协议解析
		proto_4=*(buff+ip_offset+9);
		// 长度*4
		ip_len=((*(buff+ip_offset))&0x0f)<<2;
		// ip偏移+ip长度
		tcp_offset=ip_offset+ip_len;
		//	if(proto_4!=TCP_PRO||proto_4!=UDP_PRO)return;
		//
		if(proto_4==const_var::TCP_PRO
			&&global::g_protocol==2 // 需要勾的协议是UDP
			)
		{
			goto __write;
		}
		if(proto_4==const_var::UDP_PRO&&
			global::g_protocol==1)
		{
			goto __write;
		}
		// 根据方向不同,检查的端口不同
		if(byDirect==const_var::IN_DIRECT)
			check_port=*(USHORT *)(buff+tcp_offset);
		else
			check_port=*(USHORT *)(buff+tcp_offset+2);

		check_port=ntohs(check_port);

		if(global::g_port!=0 //为0表示不检查
			&&check_port!=global::g_port)
		{
			goto __write;
		}
		//跳过不转发的端口
		for(it=global::noredirport.begin();
			it!=global::noredirport.end();
			it++)
		{
			if(check_port==*it)
				goto __write;
		}
	#if defined(DEBUG) || defined(_DEBUG)	
		EnterCriticalSection(&global::cs);
		printf("From :%d.%d.%d.%d \n",buff[ip_offset+15],buff[ip_offset+14],buff[ip_offset+13],buff[ip_offset+12]);
		printf("To :%d.%d.%d.%d \n",buff[ip_offset+19],buff[ip_offset+18],buff[ip_offset+17],buff[ip_offset+16]);
		LeaveCriticalSection(&global::cs);
	#endif
		// ip校验字位置
		*(ushort *)(buff+ip_offset+10)=0;
		// 修正ip
		if(byDirect==const_var::IN_DIRECT)
		*((uint *)(buff+ip_offset+12))=global::g_dworgIP;
		else
		*((uint *)(buff+ip_offset+16))=global::g_dwrediIP;
		// 计算校验字并回写
		sum=ip_checksum((ushort *)(buff+ip_offset),ip_len);
		*(ushort *)(buff+ip_offset+10)=sum;

		#if defined(DEBUG) || defined(_DEBUG)
		EnterCriticalSection(&global::cs);
		printf("Packet IP Redirect...\n");
		LeaveCriticalSection(&global::cs);
		#endif
		// 检查 mac地址是否需要重写
		if(global::g_bMacLocal)
		{
			ret=(byDirect==const_var::IN_DIRECT?6:0);
			memmove(buff+ret,global::g_ulMACAddr,6);
		}
		//处理tcp 和udp
		if(proto_4==const_var::TCP_PRO||proto_4==const_var::UDP_PRO)
		{

			ip_all_len=htons(*((ushort *)(buff+ip_offset+2)));
			tcp_all_len=ip_all_len-ip_len;
			//tcp是可变长度,udp是固定长度
			if(proto_4==const_var::TCP_PRO)
			tcpudp_header_len=((*(buff+tcp_offset+12))&0xf0)>>2;
			else
			tcpudp_header_len=8;
			//方向检查 并 检查是否要执行个人回调函数
			if(byDirect==const_var::IN_DIRECT&&
				global::g_InCallBack)
				global::g_InCallBack(proto_4,
				buff+tcp_offset+tcpudp_header_len,
				tcp_all_len-tcpudp_header_len);
			if(byDirect==const_var::OUT_DIRECT&&
				global::g_OutCallBack)
				global::g_OutCallBack(proto_4,
				buff+tcp_offset+tcpudp_header_len,
				tcp_all_len-tcpudp_header_len);
			// tcp校验字重算
			if(proto_4==const_var::TCP_PRO)
			{
				*(ushort *)(buff+tcp_offset+16)=0;
				//cal len ip header all length - ip header length

				sum=tcpudp_sum_calc(proto_4,tcp_all_len,
					(USHORT *)(buff+ip_offset+12),
					(USHORT *)(buff+ip_offset+16),
					(USHORT *)(buff+tcp_offset));

				*(ushort *)(buff+tcp_offset+16)=sum;
			}
			//udp
			else
			{
				*(ushort *)(buff+tcp_offset+6)=0;
				//cal len ip header all length - ip header length

				sum=tcpudp_sum_calc(proto_4,tcp_all_len,(USHORT *)(buff+ip_offset+12),
					(USHORT *)(buff+ip_offset+16),(USHORT *)(buff+tcp_offset));

				*(ushort *)(buff+tcp_offset+6)=sum;
			}
		}// ? if(tpc||udp) ?
	}// ? if (dwNumberOfBytesTransfered>???) ?

__write:
	ret=(byDirect==const_var::IN_DIRECT? \
		global::g_cpAdp->WriteOutEx(buff,dwNumberOfBytesTransfered,lpOverlapped,InIOWriteCompletionRoutine): \
		global::g_cpAdp->WriteInEx(buff,dwNumberOfBytesTransfered,lpOverlapped,OutIOWriteCompletionRoutine)
		);
	if(0==ret)
	{
		EnterCriticalSection(&global::cs);
		wprintf(L"%lsIOReadCompletionRoutine Last Error:%d\n",
			byDirect==const_var::IN_DIRECT?L"In":L"Out",GetLastError());
		LeaveCriticalSection(&global::cs);
	}
}


 VOID CALLBACK InIOReadCompletionRoutine(   
                  DWORD dwErrorCode,   
                  DWORD dwNumberOfBytesTransfered,   
                  LPOVERLAPPED lpOverlapped )
 {
	 IOReadCompletionRoutine( 
		const_var::IN_DIRECT,
		dwErrorCode,   
		dwNumberOfBytesTransfered,   
		 lpOverlapped );
 }
 VOID CALLBACK OutIOReadCompletionRoutine(   
                  DWORD dwErrorCode,   
                  DWORD dwNumberOfBytesTransfered,   
                  LPOVERLAPPED lpOverlapped )
 {
	 
	 IOReadCompletionRoutine( 
		const_var::OUT_DIRECT,
		 dwErrorCode,   
		 dwNumberOfBytesTransfered,   
		 lpOverlapped );
 }

 
 unsigned WorkFunc(byte byDirect)
 {
	 uint ret,i;
	 PKT_REDIR_FILTER_ENTRY *ent;
	 byte *(buff[0x40]),*Dbuff;//申请0x40 个 0x1000+OVERLAPPED 长度的数据块,
	 //包括 OVERLAPPED 和 数据 Dbuff 指向数据部分
	 //选择不同的钩子
	 ent=(byDirect==const_var::IN_DIRECT?
		 &global::g_Inent:&global::g_Outent
		 );
	 ret=(byDirect==const_var::IN_DIRECT?
		 global::g_cpAdp->SetInHook(ent,1):
		global::g_cpAdp->SetOutHook(ent,1)
	 );
	 if(ret!=0)
		 return 0xFF;
	 EnterCriticalSection(&global::cs);
	 wprintf(L"start %lswork\n",byDirect==const_var::IN_DIRECT?L"in":L"out");
	 LeaveCriticalSection(&global::cs);

	 for(i=0;i<0x40;i++)
	 {
		 //端口被关闭就退出
		 if((!global::g_cpAdp->isInHandleOpen())&&
			 byDirect==const_var::IN_DIRECT)break;
		 if((!global::g_cpAdp->isOutHandleOpen())&&
			 byDirect==const_var::OUT_DIRECT)break;

		 buff[i]=(byte *)malloc(0x1000+sizeof(OVERLAPPED));
		 Dbuff=buff[i]+sizeof(OVERLAPPED);
		 memset(buff[i],0,sizeof(OVERLAPPED));
		 ret=(byDirect==const_var::IN_DIRECT?
			 global::g_cpAdp->ReadInEx(Dbuff,0x1000,(LPOVERLAPPED)buff[i],InIOReadCompletionRoutine):
			 global::g_cpAdp->ReadOutEx(Dbuff,0x1000,(LPOVERLAPPED)buff[i],OutIOReadCompletionRoutine)
			 );
		 if(ret==0)
		 {
			 EnterCriticalSection(&global::cs);
			 wprintf(L"%swork  stop:%d\n",
				 byDirect==const_var::IN_DIRECT?L"in":L"out",
				 GetLastError());
			 LeaveCriticalSection(&global::cs);
			 break;
		 }
	 }
	 while(1)
	 {
		 // 睡觉
		 SleepEx(0x1f4,1);
		 //检查
		 if(global::g_cpAdp==NULL)break;
		 ret=(byDirect==const_var::IN_DIRECT?
			 global::g_cpAdp->isInHandleOpen():
			 global::g_cpAdp->isOutHandleOpen());
		 if(!ret)break;
	 }
	 for(i=0;i<0x40;i++)
	 {
		 // 异常退出就由系统回收
		 free(buff[i]);
	 }
	 return 0;
 }

static unsigned __stdcall InWork(void * pList)
{
	return WorkFunc(const_var::IN_DIRECT);
}
static unsigned __stdcall OutWork(void * pList)
{
	return WorkFunc(const_var::OUT_DIRECT);
}

/// 创建 进出线程并等待
static unsigned __stdcall MainWork(void * pList)
{
	HANDLE hlist[2]; // 进出线程的 等待
	DWORD ret; 
	unsigned  InID,OutID;
	if(!global::g_hInThread)
		global::g_hInThread=(HANDLE)_beginthreadex(NULL,0,InWork,NULL,0,&InID);
	if((!global::g_hInThread)||
		global::g_hInThread==INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	if(!global::g_hOutThread)
		global::g_hOutThread=(HANDLE)_beginthreadex(NULL,0,OutWork,NULL,0,&OutID);
	if((!global::g_hOutThread)||global::g_hOutThread==INVALID_HANDLE_VALUE)
	{
		//TerminateThread(g_hInThread,1);
		WaitForSingleObject(global::g_hInThread,
			INFINITE);
		CloseHandle(global::g_hInThread);
		global::g_hInThread=0;
		return 0;
	}
	hlist[0]=global::g_hInThread;
	hlist[1]=global::g_hOutThread;
	do{
		ret=WaitForMultipleObjectsEx(2,hlist,0,0x3e8,1);
	}while(ret==0x102);
	return 0;
}


UINT  WINAPI redirIP(const wchar_t szDevName[],const wchar_t cporIP[],const wchar_t cpreIP[],const UCHAR proto,const USHORT wport)
{
	
	int i=0,size=0,j=0;
	static int ret=0;
	const CBindList *list;
	unsigned  MainID;
	hostent * he;
	WORD wVersionRequested;
	WSADATA wsaData;
	CAutoBuffer<char> chrtmp(100);
	CAutoBuffer<wchar_t> env_noport(1000);
	wchar_t *pport;
	//wstring noport_str;
	if(IsWow64Current())
	{
		return 40;
	}
	// 从环境中获取 不转发的端口
	memset(env_noport,0,2000);
	ret=GetEnvironmentVariableW(L"noport",env_noport,1000);
	//noredirport=reg_noredirport;
	if(ret!=0)
	{
		// 中间用任意非数字字符隔开
		pport=env_noport;
		while(pport[0]>=L'0'&&pport[0]<=L'9')
		{
			global::noredirport.push_back((USHORT)wcstol(pport,&pport,10));
			if(*pport==0)break;
			pport++;
		}
	}
	InitializeCriticalSection(&global::cs);
	// 设置全局的协议和端口
	global::g_protocol=proto;
	global::g_port=wport;
	ret=DrvCall::Init();
	if(ret)return ret;
	list=CBindList::GetAllBindList();
	if(list==NULL)
	{
		return CBindList::GetError();
	}
	//判断是设备名格式
	if(wcsstr(szDevName,L"\\DEVICE\\")==szDevName)
		global::g_cpAdp=list->getByName(wstring(szDevName));
	// ip格式
	else if (wcsstr(szDevName,L".")!=NULL)
	{
		global::g_cpAdp=list->getByIP(wstring(szDevName));
	}
	// mac格式
	else
	{
		global::g_cpAdp=list->getByMac(wstring(szDevName));
		
	}
	if(global::g_cpAdp==NULL)
	{
		//if err20??
		return 20;
	}
	ret=global::g_cpAdp->OpenHandles();
	if(ret)
	{
		//err 9 In句柄获取失败
		global::g_cpAdp->CloseHandles();
		return 9;
	}
	// 开启socket
	wVersionRequested =MAKEWORD( 2, 0 );
	ret = WSAStartup( wVersionRequested, &wsaData );
	if ( ret  ) return 33;
	memset(chrtmp,0,100);
	wcstombs(chrtmp,cporIP,100);
	// 域名到ip
	he=gethostbyname(chrtmp);
	if(!he||he->h_length!=4||!he->h_addr_list||!he->h_addr_list[0])
	{
		wprintf(L"Get Ip Error:%d\n",WSAGetLastError());
		return 30;
	}
	//inet_addr
	global::g_dworgIP=*(DWORD *)(he->h_addr_list[0]);
	memset(chrtmp,0,100);
	wcstombs(chrtmp,cpreIP,100);
	he=gethostbyname(chrtmp);
	if(!he||he->h_length!=4||!he->h_addr_list||!he->h_addr_list[0])
	{
		return 31;
	}
	global::g_dwrediIP=*(DWORD *)(he->h_addr_list[0]);
	// 以下两种情况是ip解析出错了
	if(global::g_dworgIP==MAXFF||
		global::g_dwrediIP==MAXFF)
	{
		//err 11 ip输入错误
		//	printf("IP error.\n");
		return 11;
	}

	ret=6;
	// arp 请求 来处理 局域网的 mac地址替换
	if(NO_ERROR==SendARP(global::g_dwrediIP,
		INADDR_ANY,(PULONG)global::g_ulMACAddr,
		(PULONG)&ret)&&ret==6)
	{
		global::g_bMacLocal=false;
	}
	else
	{
		global::g_bMacLocal=false;
	}
	WSACleanup();
	
	// 设置hook需要的结构
	global::g_Inent.m_IPSrcAddressRangeEnd=
		ntohl(global::g_dwrediIP);
	global::g_Inent.m_IPSrcAddressRangeStart=
		ntohl(global::g_dwrediIP);
	
	global::g_Outent.m_IPDstAddressRangeEnd=
		ntohl(global::g_dworgIP);
	global::g_Outent.m_IPDstAddressRangeStart=
		ntohl(global::g_dworgIP);

	//HANDLE ev1;
	//inev=CreateEvent(0,0,0,0);

	//启动主线程
   global::g_hMainThread=
	   (HANDLE)_beginthreadex(NULL,0,MainWork,NULL,0,&MainID);

   	if((!global::g_hMainThread)||
		(global::g_hMainThread==INVALID_HANDLE_VALUE))
	{
		//err 12 主线程启动失败
		TerminateThread(global::g_hMainThread,1);
		WaitForSingleObject(global::g_hMainThread,INFINITE);
		CloseHandle(global::g_hMainThread);
		global::g_hMainThread=0;
		return 12;
	}
	return 0;
}
/// 用于dll不能初始化 全局变量
void WINAPI DllInit()
{
	using namespace global;
	g_cpAdp=NULL;
	g_hInThread=NULL;
	g_hOutThread=NULL;
	g_hMainThread=NULL;
	g_dworgIP=0;
	g_port=0;
	g_protocol=0;
	g_dwrediIP=0;

	g_Outent.m_IPSrcAddressRangeStart=
		g_Outent.m_IPDstAddressRangeStart=
		g_Inent.m_IPSrcAddressRangeStart=
		g_Inent.m_IPDstAddressRangeStart=
		g_Inent.m_nReserved=
		g_Outent.m_nReserved=0;
	g_Outent.m_IPSrcAddressRangeEnd=
		g_Outent.m_IPDstAddressRangeEnd=
		g_Inent.m_IPSrcAddressRangeEnd=
		g_Inent.m_IPDstAddressRangeEnd=MAXFF;
	g_Outent.m_nFilterAction=
		g_Inent.m_nFilterAction=REDIRECT;
	
    g_InCallBack=NULL;
	g_OutCallBack=NULL;
	g_bMacLocal=false;
	memset(g_ulMACAddr,0,8);
	noredirport.clear();
	DrvCall::initFlag=0;
	CBindList::initFlag=0;
}



BOOL WINAPI  
	DllEntry(HANDLE	hinstDLL, 
	DWORD dwReason, 
	LPVOID /* lpvReserved */)
{

	/*global init*/
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
	//case DLL_THREAD_ATTACH:
		DllInit();
		break;
	//case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		Free(1);
		break;
	default:
		break;
	}
	return TRUE;
}



int  wmain(int argc,wchar_t ** argv)
{
	//221.231.130.70
	//uchar mIP[4]={60,176,43,163};
	uint ret=0;
	int ch;
	const CBindList *list;
	USHORT mPort;
	UCHAR mPro;
	std::vector <CBindAdapter>::const_iterator it;
	//printf("%d\n",sizeof(ULONG));
	if(IsWow64Current()){
		wprintf(L"run in 64bit version..\n");
		system("pause");
		return 0;};
if(argc ==1){
	list=CBindList::GetAllBindList();
	if(list==NULL)
	{
		wprintf(L"Error No:%d\n",CBindList::GetError());
		system("pause");
		return 0;
	}
	wprintf(L"Support Adapter List:\n");
	for(it=list->getLists()->begin();it!=list->getLists()->end();it++)
	{
		if(!it->getDesc()->empty())
		wprintf(L"\t%ls\n",it->getDesc()->c_str());
		if(!it->getName()->empty())
			wprintf(L"\t%ls\n",it->getName()->c_str());
		if(!it->getIp()->empty())
		wprintf(L"\t%ls\n",it->getIp()->c_str());
		if(!it->getMac()->empty())
		wprintf(L"\t%ls\n",it->getMac()->c_str());
		wprintf(L"------------------------------------\n");
	}
	system("pause");
	return 0;
}
	mPort=0;
	mPro=0;
	while(ch=getopt_w(argc,argv,L"p:o:")!=-1)
	{
		switch(ch)
		{
		case L'p':
			mPort=(USHORT)wcstol(optarg_w,NULL,10);
			break;
		case L'o':
			mPro=(UCHAR)wcstol(optarg_w,NULL,10);
			break;
		}
	}
	if(mPro>2)
	{
		wprintf(L"Protocol 1=TCP 2=UDP 0=BOTH .\n");
		return 0;
	}

	 if((argc-optind) !=3)
	{
		wprintf(L"Arguments number is not 3.\n");
		system("pause");
		return 0;
	}
	 signal(SIGINT,signalExit);
	 signal(SIGABRT,signalExit);
	 signal(SIGTERM,signalExit);
	atexit(AtExit);
	if(ret=redirIP(argv[optind],argv[optind+1],argv[optind+2],mPro,mPort))
	{
		wprintf(L"Error Back:%d\n",ret);
	}
	wprintf(L"Running\n");
		if(global::g_hMainThread){
			WaitForSingleObject(global::g_hMainThread,INFINITE);
		}
	
	wprintf(L"End\n");
	system("pause");
	return 0;
}

void WINAPI RegisterNoPort(const USHORT * pPort,DWORD dSize)
{
	DWORD i;
	global::noredirport.clear();
	if(pPort==NULL||dSize==0)return;
	for(i=0;i<dSize;i++)
	{
		if(pPort[i]!=0)
		global::noredirport.push_back(pPort[i]);
	}
}
void WINAPI RegisterInCallBack(const MyCallBack m)
{
	global::g_InCallBack=m;
}
void WINAPI RegisterOutCallBack(const MyCallBack m)
{
	global::g_OutCallBack=m;
}