#include "main.h"
#include "stdio.h"
#include "ntddndis.h"
#include "stdlib.h"
#include <process.h>
#include "winsock.h"
#include <string.h>
#include <iphlpapi.h>
#include "DrvCall.h"
#include "BindAdapter.h"
#include "BindList.h"
#include "vector"

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


static HANDLE InHandle=NULL,InThread;
static HANDLE OutHandle=NULL,OutThread;
static HANDLE MainThread=NULL;
unsigned  InID,OutID,MainID;
//static byte Inbuff[0x1000];
//static byte Outbuff[0x1000];
static  uint orgIP=0;//={221,231,130,70};

//static const BYTE sdip[4]={207,56,113,28};
static const char *DevName=0;
static const char *vDevName=0;
static uint rediIP=0;

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


static PKT_REDIR_FILTER_ENTRY outent={0,MAXFF,0,MAXFF,REDIRECT,0};
static PKT_REDIR_FILTER_ENTRY inent={0,MAXFF,0,MAXFF,REDIRECT,0};

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
    	while (sum>>16)
		sum = (sum & 0xFFFF)+(sum >> 16);
	//sum = (sum >> 16) + (sum & 0xffff);
	//sum += (sum >> 16);
	// Take the one's complement of sum
return (USHORT)(~sum);
}
void AtExit(void)
{
	Free();
}
 void  WINAPI Free()
{
	DWORD code,ret;
	DeleteCriticalSection(&cs);
	DrvCall::Init();
	if(InHandle!=NULL)
	{

		DrvCall::ResetPktRedirFilter(InHandle);
		CloseHandle(InHandle);
		InHandle=0;
	}
	if(OutHandle!=NULL)
	{

		DrvCall::ResetPktRedirFilter(OutHandle);
		CloseHandle(OutHandle);
		OutHandle=0;
	}
	ret=GetExitCodeThread(InThread,&code);
	if(ret==STILL_ACTIVE)
	{
		TerminateThread(InThread,1);
		CloseHandle(InThread);
		InThread=0;
	}
	ret=GetExitCodeThread(OutThread,&code);
	if(ret==STILL_ACTIVE)
	{
		TerminateThread(OutThread,1);
		CloseHandle(OutThread);
		OutThread=0;
	}
	ret=GetExitCodeThread(MainThread,&code);
	if(ret==STILL_ACTIVE)
	{
		TerminateThread(MainThread,1);
		CloseHandle(MainThread);
		MainThread=0;
	}
	DrvCall::Free();
}
VOID CALLBACK InIOWriteCompletionRoutine(   
                  DWORD dwErrorCode,   
                  DWORD dwNumberOfBytesTransfered,   
                  LPOVERLAPPED lpOverlapped )
 {
	byte *Inbuff=(byte *)lpOverlapped+sizeof(OVERLAPPED);
	 if(dwErrorCode!=0)return;
	 if(ReadFileEx(InHandle,Inbuff,0x1000,lpOverlapped,InIOReadCompletionRoutine)==0)
	 {
		 EnterCriticalSection(&cs);
		 printf("InIOWriteCompletionRoutine Last Error:%d\n",GetLastError());
		 LeaveCriticalSection(&cs);
	 }
	 //IO Finish
	 
 }
VOID CALLBACK OutIOWriteCompletionRoutine(   
                  DWORD dwErrorCode,   
                  DWORD dwNumberOfBytesTransfered,   
                  LPOVERLAPPED lpOverlapped )
 {

	byte *Outbuff=(byte *)lpOverlapped+sizeof(OVERLAPPED);
	 if(dwErrorCode!=0)return;
	 if(0==ReadFileEx(OutHandle,Outbuff,0x1000,lpOverlapped,OutIOReadCompletionRoutine))
	 {
		 EnterCriticalSection(&cs);
		 printf("OutIOWriteCompletionRoutine Last Error:%d\n",GetLastError());
		 LeaveCriticalSection(&cs);
	 }
	 //IO Finish
	 
 }
 VOID CALLBACK InIOReadCompletionRoutine(   
                  DWORD dwErrorCode,   
                  DWORD dwNumberOfBytesTransfered,   
                  LPOVERLAPPED lpOverlapped )
 {
	 ushort sum=0,i,j=0;
	 byte *Inbuff=(byte *)lpOverlapped+sizeof(OVERLAPPED);
	// printf("InIO\n");
	  if(dwNumberOfBytesTransfered>=34)
	 {
	 if(dwErrorCode!=0)return;
	 if(Inbuff[14]==0x11)i=8;
	 else if(Inbuff[14]==0x45) i=0;
	 else i=-1;
	 if(i>=0)
	 {
	//	 EnterCriticalSection(&cs);
		// printf("InIo\n");
	//	 LeaveCriticalSection(&cs);		 
	 
	 *(ushort *)(Inbuff+14+i+10)=0;

	 *((uint *)(Inbuff+14+i+12))=orgIP;
	 sum=ip_checksum((ushort *)(Inbuff+14+i),20);
	 *(ushort *)(Inbuff+14+i+10)=sum;

	 if(dwNumberOfBytesTransfered>(i+34)&&*(Inbuff+14+i+9)==6)
	 {

		*(ushort *)(Inbuff+14+i+20+16)=0;
		sum=tcp_sum_calc(dwNumberOfBytesTransfered-34-i,(USHORT *)(Inbuff+14+i+12),(USHORT *)(Inbuff+14+i+16),(USHORT *)(Inbuff+14+i+20));
		*(ushort *)(Inbuff+14+i+20+16)=sum+0x0600;//i don't know why ??
	 }
	 }
	  }

	 if(0==WriteFileEx(OutHandle,Inbuff,dwNumberOfBytesTransfered,lpOverlapped,InIOWriteCompletionRoutine))
		 {
		  EnterCriticalSection(&cs);
		  printf("InIOReadCompletionRoutine Last Error:%d\n",GetLastError());
		  LeaveCriticalSection(&cs);
	 }
 }
 VOID CALLBACK OutIOReadCompletionRoutine(   
                  DWORD dwErrorCode,   
                  DWORD dwNumberOfBytesTransfered,   
                  LPOVERLAPPED lpOverlapped )
 {
	 
	 ushort sum=0,i;
	 byte *Outbuff=(byte *)lpOverlapped+sizeof(OVERLAPPED),*tcpbuff=NULL;
	 
	 if(dwErrorCode!=0)return;
	//printf("OutIO\n");
	 if(dwNumberOfBytesTransfered>=34)
	 {
		 if(Outbuff[14]==0x11)i=8;
		 else if(Outbuff[14]==0x45)i=0;
		 else i=-1;
if(i>=0)
	 {

	 *(ushort *)(Outbuff+14+i+10)=0;
	 *((uint *)(Outbuff+14+i+16))=rediIP;
	 sum=ip_checksum((ushort *)(Outbuff+14+i),20);
	 *(ushort *)(Outbuff+14+i+10)=sum;
	 if(dwNumberOfBytesTransfered>(i+34)&&*(Outbuff+14+i+9)==6)
	 {
		 *(ushort *)(Outbuff+14+i+20+16)=0;
		 sum=tcp_sum_calc(dwNumberOfBytesTransfered-34-i,(USHORT *)(Outbuff+14+i+12),(USHORT *)(Outbuff+14+i+16),(USHORT *)(Outbuff+14+i+20));
		 *(ushort *)(Outbuff+14+i+20+16)=sum;
	 }
	}
	}

	 if(0==WriteFileEx(InHandle,Outbuff,dwNumberOfBytesTransfered,lpOverlapped,OutIOWriteCompletionRoutine))
		 {
		EnterCriticalSection(&cs);
		printf("OutIOReadCompletionRoutine Last Error:%d\n",GetLastError()) ;
		LeaveCriticalSection(&cs);
		}
 }
static unsigned __stdcall InWork(void * pList)
{
	uint ret,i;
	byte *(buff[0x40]),*Inbuff;
	ret=DrvCall::SetPktRedirFilter(InHandle,&inent,1);
	if(ret!=0)
		return 0xFF;
	EnterCriticalSection(&cs);
	printf("start inwork\n");
	LeaveCriticalSection(&cs);
	for(i=0;i<0x40;i++)
	{
		if(InHandle==0)break;
		buff[i]=(byte *)malloc(0x1000+sizeof(OVERLAPPED));//让系统自动回收
		Inbuff=buff[i]+sizeof(OVERLAPPED);
		memset(buff[i],0,sizeof(OVERLAPPED));
		ret=ReadFileEx(InHandle,Inbuff,0x1000,(LPOVERLAPPED)buff[i],InIOReadCompletionRoutine);
		//printf("inwork:%d\n",ret);
		if(ret==0)
		{
			EnterCriticalSection(&cs);
			printf("inwork  stop:%d\n",GetLastError());
			LeaveCriticalSection(&cs);
			break;
		}
	}
	while(1)
	{
		SleepEx(0x1f4,1);
		if(InHandle==0)break;
	}
		for(i=0;i<0x40;i++)
	{
			free(buff[i]);
		}
	//_endthread();
	return 0;
}
static unsigned __stdcall OutWork(void * pList)
{
	uint ret,i;
	byte *(buff[0x40]),*Outbuff;
	ret=DrvCall::SetPktRedirFilter(OutHandle,&outent,1);
	if(ret!=0)
	{
		return 0xFF;
	}	
	EnterCriticalSection(&cs);
	printf("start outwork\n");
	LeaveCriticalSection(&cs);
	for(i=0;i<0x40;i++)
	{
		if(OutHandle==0)break;
		buff[i]=(byte *) malloc(0x1000+sizeof(OVERLAPPED));
		Outbuff=buff[i]+sizeof(OVERLAPPED);
		memset(buff[i],0,sizeof(OVERLAPPED));
		ret=ReadFileEx(OutHandle,Outbuff,0x1000,(LPOVERLAPPED)buff[i],OutIOReadCompletionRoutine);
		//printf("outwork:%d\n",ret);
		if(ret==0)
		{
			EnterCriticalSection(&cs);
			printf("outwork  stop:%d\n",GetLastError());
			LeaveCriticalSection(&cs);
			break;
		}
	}
		while(1)
	{
		SleepEx(0x1f4,1);
		if(OutHandle==0)break;
	}
		for(i=0;i<0x40;i++)
	{
			free(buff[i]);
		}
	//_endthread();
	return 0;
}
static unsigned __stdcall MainWork(void * pList)
{
	HANDLE hlist[2];
	DWORD ret;
	if(!InThread)
	InThread=(HANDLE)_beginthreadex(NULL,0,InWork,NULL,0,&InID);
	if(!OutThread)
	OutThread=(HANDLE)_beginthreadex(NULL,0,OutWork,NULL,0,&OutID);
	if(!InThread)
	{
		if(OutThread)
		{
			TerminateThread(OutThread,1);
			WaitForSingleObject(OutThread,INFINITE);
			OutThread=0;
		}
		return 0;
	}
	if(!OutThread)
	{
		TerminateThread(InThread,1);
		WaitForSingleObject(InThread,INFINITE);
		InThread=0;
		return 0;
	}
	hlist[0]=InThread;
	hlist[1]=OutThread;
	do{
	ret=WaitForMultipleObjectsEx(2,hlist,0,0x3e8,1);
	}while(ret==0x102);
//	_endthread();
	return 0;
}

UINT  WINAPI redirIP(const char S[],const char orIP[],const char reIP[])
{
	
	int i=0,size=0,j=0,ret=0;
	BindList *list;
	BindAdapter *adp;
	InitializeCriticalSection(&cs);
	ret=DrvCall::Init();
	if(ret)return ret;
	list=BindList::getAllBindList();
	if(list==NULL)
	{
		return BindList::GetError();
	}
	if(strstr(S,"\\DEVICE\\")==S)
		adp=list->getByName(string(S));
	else if (strstr(S,".")!=NULL)
	{
		adp=list->getByIP(string(S));
	}
	else
	{
		adp=list->getByMac(string(S));
		
	}
	if(adp==NULL)
	{
		//if err20??
		return 20;
	}
	DevName=adp->getName()->c_str();
	vDevName=adp->getvName()->c_str();
	InHandle=DrvCall::OpenLowerAdapter(DevName);
//	pPCASIM_OpenVirtualAdapter(DevName);
//	OutHandle=pPCASIM_OpenLowerAdapter(DevName);
	OutHandle=DrvCall::OpenVirtualAdapter(vDevName);
//	printf("handle :%d,%d\n",InHandle,OutHandle);
	if(InHandle==NULL||InHandle==INVALID_HANDLE_VALUE)
	{
		//err 9 In句柄获取失败
		InHandle=NULL;
		return 9;
	}
	if(OutHandle==NULL||OutHandle==INVALID_HANDLE_VALUE)
	{
		//err 10 out句柄获取失败
		OutHandle=NULL;
		return 10;
	}
	
	orgIP=inet_addr(orIP);
	rediIP=inet_addr(reIP);
	if(orgIP==MAXFF||rediIP==MAXFF)
	{
		//err 11 ip输入错误
	//	printf("IP error.\n");
		return 11;
	}
	inent.m_IPSrcAddressRangeEnd=ntohl(rediIP);
	inent.m_IPSrcAddressRangeStart=ntohl(rediIP);
	
	
	outent.m_IPDstAddressRangeEnd=ntohl(orgIP);
	outent.m_IPDstAddressRangeStart=ntohl(orgIP);

	//HANDLE ev1;
	//inev=CreateEvent(0,0,0,0);
   MainThread=(HANDLE)_beginthreadex(NULL,0,MainWork,NULL,0,&MainID);
   	if((!MainThread)||(MainThread==INVALID_HANDLE_VALUE))
	{
		//err 12 主线程启动失败
		TerminateThread(MainThread,1);
		WaitForSingleObject(MainThread,INFINITE);
		MainThread=0;
		return 12;
	}
	return 0;
}



int  main(int argc,char ** argv)
{
	//221.231.130.70
	//uchar mIP[4]={60,176,43,163};
	uint ret=0;
	BindList *list;
	std::vector <BindAdapter>::iterator it;
	//printf("%d\n",sizeof(ULONG));
	

if(argc ==1){
	list=BindList::getAllBindList();
	if(list==NULL)
	{
		printf("Error\n");
		return 0;
	}
	printf("Support Adapter List:\n");
	for(it=list->getLists()->begin();it!=list->getLists()->end();it++)
	{
		if(!it->getDesc()->empty())
		printf("\t%s\n",it->getDesc()->c_str());
		if(!it->getName()->empty())
			printf("\t%s\n",it->getName()->c_str());
		if(!it->getIp()->empty())
		printf("\t%s\n",it->getIp()->c_str());
		if(!it->getMac()->empty())
		printf("\t%s\n",it->getMac()->c_str());
		printf("------------------------------------\n");
	}
	system("pause");
	return 0;
}
	 if(argc !=4)
	{
		printf("Arguments number is not 4.\n");
		system("pause");
		return 0;
	}
	atexit(AtExit);
	if(ret=redirIP(argv[1],argv[2],argv[3]))
	{
		printf("Error Back:%d\n",ret);
		
	}
	else
	{
		printf("Running \n");
		if(MainThread){
			WaitForSingleObject(MainThread,INFINITE);
		}
	}
	printf("End\n");
	system("pause");
	return 0;
}