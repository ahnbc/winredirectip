#include "main.h"
#include "stdio.h"

#include "stdlib.h"
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

const BindAdapter *g_cpAdp=NULL;
static HANDLE g_hInThread=NULL;
static HANDLE g_hOutThread=NULL;
static HANDLE g_hMainThread=NULL;
//unsigned  InID,OutID,MainID;
//static byte Inbuff[0x1000];
//static byte Outbuff[0x1000];
static  uint g_dworgIP=0;//={221,231,130,70};
static USHORT g_port=0;
static UCHAR g_protocol=0;
//static const BYTE sdip[4]={207,56,113,28};
static uint g_dwrediIP=0;

static CRITICAL_SECTION cs;
#define MAXFF 0xFFFFFFFF
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


static PKT_REDIR_FILTER_ENTRY g_Outent={0,MAXFF,0,MAXFF,REDIRECT,0};
static PKT_REDIR_FILTER_ENTRY g_Inent={0,MAXFF,0,MAXFF,REDIRECT,0};

const byte IN_DIRECT=1,OUT_DIRECT=2;

typedef struct
{
	unsigned long saddr; //源IP地址      
	unsigned long daddr; //目的IP地址       
	uchar mbz;                   // mbz = must be zero, 用于填充对齐
	uchar protocal;             //8位协议号       
	unsigned short tcpl;    // TCP包长度      
}psdheader_t;


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


u16 tcp_sum_calc(u16 len_tcp, u16 src_addr[],u16 dest_addr[],  u16 buff[])
{
u16 prot_tcp=0x0600;
u32 sum=0;	
u16 i=len_tcp;	
	// Find out if the length of data is even or odd number. If odd,
	// add a padding byte = 0 at the end of packet
	//initialize sum to zero

while (i > 1) {
	//printf("%4.4X ",*buff);
	sum += (unsigned long)(*buff++);
	i -= sizeof(USHORT);
}
//printf("\n%d\n",len_tcp);
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

BOOL IsWow64Current()  
{  
	FARPROC fnIsWow64Process;  
	BOOL bIsWow64;  


	bIsWow64 = FALSE;  
	fnIsWow64Process = GetProcAddress(GetModuleHandleA("kernel32.dll"), "IsWow64Process");  


	if(fnIsWow64Process)  
		if(((BOOL (WINAPI *)(HANDLE, PBOOL))fnIsWow64Process)(GetCurrentProcess(), &bIsWow64))  
			return bIsWow64;  
	return FALSE;  
}


void AtExit(void)
{
	Free(1);
}
 UINT  WINAPI Free(const byte freelib)
{
	DWORD code;
	static int ret=0;
	DeleteCriticalSection(&cs);
	ret=DrvCall::Init();
	if(ret)
		return ret;
	if(g_cpAdp!=NULL){
	g_cpAdp->ResetHook();
	g_cpAdp->BeginRequest()->CloseHandles();
	g_cpAdp=NULL;
	}
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
	 ret=(byDirect==IN_DIRECT? \
		 g_cpAdp->ReadInEx(buff,0x1000,lpOverlapped,InIOReadCompletionRoutine): \
		 g_cpAdp->ReadOutEx(buff,0x1000,lpOverlapped,OutIOReadCompletionRoutine)
		 );
	 if(ret==0)
	 {
		 EnterCriticalSection(&cs);
		 printf("%sIOWriteCompletionRoutine Last Error:%d\n",byDirect==IN_DIRECT?"In":"Out",GetLastError());
		 LeaveCriticalSection(&cs);
	 }
	 //IO Finish
 }
 VOID CALLBACK InIOWriteCompletionRoutine(   
                  DWORD dwErrorCode,   
                  DWORD dwNumberOfBytesTransfered,   
                  LPOVERLAPPED lpOverlapped )
 {
	
	 IOWriteCompletionRoutine( IN_DIRECT,dwErrorCode,   
		 dwNumberOfBytesTransfered,   
		 lpOverlapped );
	 
 }
VOID CALLBACK OutIOWriteCompletionRoutine(   
                  DWORD dwErrorCode,   
                  DWORD dwNumberOfBytesTransfered,   
                  LPOVERLAPPED lpOverlapped )
 {

	 IOWriteCompletionRoutine(OUT_DIRECT,dwErrorCode,   
		 dwNumberOfBytesTransfered,   
		 lpOverlapped );
	 
 }
const static USHORT IP_PRO=0x0008,
		 PPPOE_PRO=0x6488,PPPOE_IP_PRO=0x2100,
		 LEN_ENT=14,LEN_PPPOE=8;
const static UCHAR TCP_PRO=6,UDP_PRO=17;



void  IOReadCompletionRoutine(   
	byte byDirect,
	DWORD dwErrorCode,   
	DWORD dwNumberOfBytesTransfered,   
	LPOVERLAPPED lpOverlapped )
{
	ushort sum=0,proto_3=0,ppp_next_pro=0,proto_4=0,
		ip_offset=0,tcp_offset=0,tcp_all_len=0,ip_all_len;
	byte *buff=(byte *)lpOverlapped+sizeof(OVERLAPPED);
	UCHAR ip_len;
	UINT ret;
	if(dwErrorCode!=0)return;
	if(dwNumberOfBytesTransfered>=(14+20))
	{

		proto_3=*(USHORT * )(buff+12);
		if(proto_3==IP_PRO)
			ip_offset=LEN_ENT;
		else if(proto_3==PPPOE_PRO)
		{
			ppp_next_pro=*(USHORT *)(buff+LEN_ENT+6);
			if(ppp_next_pro==PPPOE_IP_PRO)ip_offset=LEN_ENT+LEN_PPPOE;
			else goto __write;
		}
		else 	goto __write;
		proto_4=*(buff+ip_offset+9);
		ip_len=((*(buff+ip_offset))&0x0f)<<2;
		tcp_offset=ip_offset+ip_len;
		//	if(proto_4!=TCP_PRO||proto_4!=UDP_PRO)return;
		//
		if(proto_4==TCP_PRO&&g_protocol==2)
		{
			goto __write;
		}
		if(proto_4==UDP_PRO&&g_protocol==1)
		{
			goto __write;
		}
		if(g_port!=0&&*(USHORT *)(buff+tcp_offset)!=ntohs(g_port))
		{
			goto __write;
		}


		*(ushort *)(buff+ip_offset+10)=0;
		if(byDirect==IN_DIRECT)
		*((uint *)(buff+ip_offset+12))=g_dworgIP;
		else
		*((uint *)(buff+ip_offset+16))=g_dwrediIP;

		sum=ip_checksum((ushort *)(buff+ip_offset),ip_len);
		*(ushort *)(buff+ip_offset+10)=sum;

		if(proto_4==TCP_PRO)
		{

			ip_all_len=htons(*((ushort *)(buff+ip_offset+2)));
			tcp_all_len=ip_all_len-ip_len;
			//


			*(ushort *)(buff+tcp_offset+16)=0;
			//cal len ip header all length - ip header length

			sum=tcp_sum_calc(tcp_all_len,(USHORT *)(buff+ip_offset+12),
				(USHORT *)(buff+ip_offset+16),(USHORT *)(buff+tcp_offset));

			*(ushort *)(buff+tcp_offset+16)=sum;
		}

	}

__write:
	ret=(byDirect==IN_DIRECT? \
		g_cpAdp->WriteOutEx(buff,dwNumberOfBytesTransfered,lpOverlapped,InIOWriteCompletionRoutine): \
		g_cpAdp->WriteInEx(buff,dwNumberOfBytesTransfered,lpOverlapped,OutIOWriteCompletionRoutine)
		);
	if(0==ret)
	{
		EnterCriticalSection(&cs);
		printf("%sIOReadCompletionRoutine Last Error:%d\n",byDirect==IN_DIRECT?"In":"Out",GetLastError());
		LeaveCriticalSection(&cs);
	}
}


 VOID CALLBACK InIOReadCompletionRoutine(   
                  DWORD dwErrorCode,   
                  DWORD dwNumberOfBytesTransfered,   
                  LPOVERLAPPED lpOverlapped )
 {
	 IOReadCompletionRoutine( 
		IN_DIRECT,
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
		 OUT_DIRECT,
		 dwErrorCode,   
		 dwNumberOfBytesTransfered,   
		 lpOverlapped );
 }

 
 unsigned WorkFunc(byte byDirect)
 {
	 uint ret,i;
	 PKT_REDIR_FILTER_ENTRY *ent;
	 byte *(buff[0x40]),*Dbuff;
	 ent=(byDirect==IN_DIRECT?
		 &g_Inent:&g_Outent
		 );
	 ret=(byDirect==IN_DIRECT?
		 g_cpAdp->SetInHook(ent,1):
		g_cpAdp->SetOutHook(ent,1)
	 );
	 if(ret!=0)
		 return 0xFF;
	 EnterCriticalSection(&cs);
	 printf("start %swork\n",byDirect==IN_DIRECT?"in":"out");
	 LeaveCriticalSection(&cs);
	 for(i=0;i<0x40;i++)
	 {
		 if(!g_cpAdp->isInHandleOpen())break;
		 buff[i]=(byte *)malloc(0x1000+sizeof(OVERLAPPED));
		 Dbuff=buff[i]+sizeof(OVERLAPPED);
		 memset(buff[i],0,sizeof(OVERLAPPED));
		 ret=(byDirect==IN_DIRECT?
			 g_cpAdp->ReadInEx(Dbuff,0x1000,(LPOVERLAPPED)buff[i],InIOReadCompletionRoutine):
			 g_cpAdp->ReadOutEx(Dbuff,0x1000,(LPOVERLAPPED)buff[i],OutIOReadCompletionRoutine)
			 );
		 if(ret==0)
		 {
			 EnterCriticalSection(&cs);
			 printf("%swork  stop:%d\n",byDirect==IN_DIRECT?"in":"out",GetLastError());
			 LeaveCriticalSection(&cs);
			 break;
		 }
	 }
	 while(1)
	 {
		 SleepEx(0x1f4,1);
		 if(g_cpAdp==NULL)break;
		 ret=(byDirect==IN_DIRECT?
			 g_cpAdp->isInHandleOpen():
			 g_cpAdp->isOutHandleOpen());
		 if(!ret)break;
	 }
	 for(i=0;i<0x40;i++)
	 {
		 free(buff[i]);
	 }
	 return 0;
 }

static unsigned __stdcall InWork(void * pList)
{
	return WorkFunc(IN_DIRECT);
}
static unsigned __stdcall OutWork(void * pList)
{
	return WorkFunc(OUT_DIRECT);
}

static unsigned __stdcall MainWork(void * pList)
{
	HANDLE hlist[2];
	DWORD ret;
	unsigned  InID,OutID;
	if(!g_hInThread)
	g_hInThread=(HANDLE)_beginthreadex(NULL,0,InWork,NULL,0,&InID);
	if((!g_hInThread)||g_hInThread==INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	if(!g_hOutThread)
	g_hOutThread=(HANDLE)_beginthreadex(NULL,0,OutWork,NULL,0,&OutID);
	if((!g_hOutThread)||g_hOutThread==INVALID_HANDLE_VALUE)
	{
		//TerminateThread(g_hInThread,1);
		WaitForSingleObject(g_hInThread,INFINITE);
		CloseHandle(g_hInThread);
		g_hInThread=0;
		return 0;
	}
	hlist[0]=g_hInThread;
	hlist[1]=g_hOutThread;
	do{
	ret=WaitForMultipleObjectsEx(2,hlist,0,0x3e8,1);
	}while(ret==0x102);
	return 0;
}

UINT  WINAPI redirIP(const wchar_t szDevName[],const wchar_t cporIP[],const wchar_t cpreIP[],const UCHAR proto,const USHORT wport)
{
	
	int i=0,size=0,j=0;
	static int ret=0;
	BindList *list;
	unsigned  MainID;
	hostent * he;
	WORD wVersionRequested;
	WSADATA wsaData;
	char * chrtmp;
	if(IsWow64Current())return 40;
	InitializeCriticalSection(&cs);
	g_protocol=proto;
	g_port=wport;
	ret=DrvCall::Init();
	if(ret)return ret;
	list=BindList::getAllBindList();
	if(list==NULL)
	{
		return BindList::GetError();
	}
	if(wcsstr(szDevName,L"\\DEVICE\\")==szDevName)
		g_cpAdp=list->getByName(wstring(szDevName));
	else if (wcsstr(szDevName,L".")!=NULL)
	{
		g_cpAdp=list->getByIP(wstring(szDevName));
	}
	else
	{
		g_cpAdp=list->getByMac(wstring(szDevName));
		
	}
	if(g_cpAdp==NULL)
	{
		//if err20??
		return 20;
	}
	ret=g_cpAdp->BeginRequest()->OpenHandles();
	//DevName=g_cpAdp->getName()->c_str();
	//vDevName=g_cpAdp->getvName()->c_str();
	//InHandle=DrvCall::OpenLowerAdapter(DevName);
//	pPCASIM_OpenVirtualAdapter(DevName);
//	OutHandle=pPCASIM_OpenLowerAdapter(DevName);
	//OutHandle=DrvCall::OpenVirtualAdapter(vDevName);
//	printf("handle :%d,%d\n",InHandle,OutHandle);
	if(ret)
	{
		//err 9 In句柄获取失败
		g_cpAdp->BeginRequest()->CloseHandles();
		return 9;
	}
	wVersionRequested =MAKEWORD( 2, 0 );
	ret = WSAStartup( wVersionRequested, &wsaData );
	if ( ret  ) return 33;
	chrtmp=(char *)malloc(100);
	memset(chrtmp,0,100);
	wcstombs(chrtmp,cporIP,100);
	he=gethostbyname(chrtmp);
	if(!he||he->h_length!=4||!he->h_addr_list||!he->h_addr_list[0])
	{
		wprintf(L"Get Ip Error:%d\n",WSAGetLastError());
		return 30;
	}
	//inet_addr
	g_dworgIP=*(DWORD *)(he->h_addr_list[0]);
	memset(chrtmp,0,100);
	wcstombs(chrtmp,cpreIP,100);
	he=gethostbyname(chrtmp);
	if(!he||he->h_length!=4||!he->h_addr_list||!he->h_addr_list[0])
	{
		return 31;
	}
	g_dwrediIP=*(DWORD *)(he->h_addr_list[0]);
	free(chrtmp);
	WSACleanup();
	if(g_dworgIP==MAXFF||g_dwrediIP==MAXFF)
	{
		//err 11 ip输入错误
	//	printf("IP error.\n");
		return 11;
	}
	g_Inent.m_IPSrcAddressRangeEnd=ntohl(g_dwrediIP);
	g_Inent.m_IPSrcAddressRangeStart=ntohl(g_dwrediIP);
	
	g_Outent.m_IPDstAddressRangeEnd=ntohl(g_dworgIP);
	g_Outent.m_IPDstAddressRangeStart=ntohl(g_dworgIP);

	//HANDLE ev1;
	//inev=CreateEvent(0,0,0,0);
	//MainWork(NULL);
   g_hMainThread=(HANDLE)_beginthreadex(NULL,0,MainWork,NULL,0,&MainID);
   	if((!g_hMainThread)||(g_hMainThread==INVALID_HANDLE_VALUE))
	{
		//err 12 主线程启动失败
		TerminateThread(g_hMainThread,1);
		WaitForSingleObject(g_hMainThread,INFINITE);
		CloseHandle(g_hMainThread);
		g_hMainThread=0;
		return 12;
	}
	return 0;
}
void WINAPI DllInit()
{
	g_cpAdp=NULL;
	g_hInThread=NULL;
	g_hOutThread=NULL;
	g_hMainThread=NULL;
	g_dworgIP=0;
	g_port=0;
	g_protocol=0;
	g_dwrediIP=0;

	g_Outent.m_IPSrcAddressRangeStart=g_Outent.m_IPDstAddressRangeStart=
		g_Inent.m_IPSrcAddressRangeStart=g_Inent.m_IPDstAddressRangeStart=
		g_Inent.m_nReserved=g_Outent.m_nReserved=0;
	g_Outent.m_IPSrcAddressRangeEnd=g_Outent.m_IPDstAddressRangeEnd=
		g_Inent.m_IPSrcAddressRangeEnd=g_Inent.m_IPDstAddressRangeEnd=MAXFF;
		g_Outent.m_nFilterAction=g_Inent.m_nFilterAction=REDIRECT;

	DrvCall::initFlag=0;
	BindList::initFlag=0;
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
	BindList *list;
	USHORT mPort;
	UCHAR mPro;
	std::vector <BindAdapter>::const_iterator it;
	//printf("%d\n",sizeof(ULONG));
	if(IsWow64Current()){
		printf("run in 64bit version..\n");
		system("pause");
		return 0;};
if(argc ==1){
	list=BindList::getAllBindList();
	if(list==NULL)
	{
		wprintf(L"Error No:%d\n",BindList::GetError());
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
	atexit(AtExit);
	//printf("Running \n");
	if(ret=redirIP(argv[optind],argv[optind+1],argv[optind+2],mPro,mPort))
	{
		wprintf(L"Error Back:%d\n",ret);
	}
	{
		    wprintf(L"Running \n");
		if(g_hMainThread){
			WaitForSingleObject(g_hMainThread,INFINITE);
		}
	}
	wprintf(L"End\n");
	system("pause");
	return 0;
}