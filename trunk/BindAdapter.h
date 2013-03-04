#ifndef _H_CLS_BINDAPT
#define  _H_CLS_BINDAPT
#include <string>
#include "main.h"
using namespace std;

class BindAdapter
{
public:
	BindAdapter(const char *vname,const char *name);
	static UINT strMacConv(string,USHORT *) ;
	static UINT MactoStr(USHORT *,char *) ;

	const string * getvName() const{return &vAdapter;};
	const string * getName() const{return &Adapter;};
	const string * getMac() const{return &Mac_str;};
	const string * getDesc() const{return &Desc;};
	const string * getIp() const{return &IP;};
	UINT getType() const {return type;};

	void setvName(const char * s){vAdapter=s;};
	void setName(const char *s){Adapter=s;};
	void setIp(const char *s){IP=s;};
	void setMac(const USHORT *);
	void setDesc(const char *s){Desc=s ;};
	void setType(UINT s){type=s ;};

	BindAdapter * BeginRequest() const{return const_cast<BindAdapter *>(this);}; 


	BOOL isSameMac(string s) const{return Mac_str==s; };
	BOOL isSameMac(USHORT *) const;
	BOOL isSameIP(string s) const{ return IP==s;};

	UINT OpenHandles();
	UINT OpenInBound();
	UINT OpenOutBound();

	UINT ResetHook() const;
	UINT ResetInHook() const;
	UINT ResetOutHook() const;

	UINT SetInHook(PPKT_REDIR_FILTER_ENTRY pPktRedirFilterList, 
		ULONG nPktRedirFilterListEntryCount ) const;
	UINT SetOutHook(PPKT_REDIR_FILTER_ENTRY pPktRedirFilterList, 
		ULONG nPktRedirFilterListEntryCount ) const;

   
	UINT CloseHandles() ;
	UINT CloseInHandle() ;
	UINT CloseOutHandle() ;


	BOOL isOutHandleOpen() const {return OutBound!=NULL;};
	BOOL isInHandleOpen() const {return InBound!=NULL;};


	UINT WriteInEx(LPCVOID lpBuffer,
		 DWORD nNumberOfBytesToWrite,
		LPOVERLAPPED lpOverlapped,
		LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)const
	{
		if(InBound==NULL){SetLastError(RPC_X_NULL_REF_POINTER);return 0;}
		return WriteFileEx(InBound,lpBuffer,nNumberOfBytesToWrite,lpOverlapped,lpCompletionRoutine);
	}
	;
	UINT WriteOutEx(LPCVOID lpBuffer,
		DWORD nNumberOfBytesToWrite,
		LPOVERLAPPED lpOverlapped,
		LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)const
	{
		if(OutBound==NULL){SetLastError(RPC_X_NULL_REF_POINTER);return 0;}
		return WriteFileEx(OutBound,lpBuffer,nNumberOfBytesToWrite,lpOverlapped,lpCompletionRoutine);
	};
	UINT WriteIn(LPCVOID lpBuffer,
		DWORD nNumberOfBytesToWrite,
		LPDWORD lpNumberOfBytesWritten, 
		LPOVERLAPPED lpOverlapped)const
	{
		if(InBound==NULL){SetLastError(RPC_X_NULL_REF_POINTER);return 0;}
		return WriteFile(InBound,lpBuffer,nNumberOfBytesToWrite,lpNumberOfBytesWritten, lpOverlapped);
	}
	;
	UINT WriteOut(LPCVOID lpBuffer,
		DWORD nNumberOfBytesToWrite,
		LPDWORD lpNumberOfBytesWritten, 
		LPOVERLAPPED lpOverlapped)const
	{
		if(OutBound==NULL){SetLastError(RPC_X_NULL_REF_POINTER);return 0;}
		return WriteFile(OutBound,lpBuffer,nNumberOfBytesToWrite,lpNumberOfBytesWritten, lpOverlapped);
	};


	UINT ReadInEx(LPVOID lpBuffer,
		DWORD nNumberOfBytesToWrite,
		LPOVERLAPPED lpOverlapped,
		LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)const
	{
		if(InBound==NULL){SetLastError(RPC_X_NULL_REF_POINTER);return 0;}
		return ReadFileEx(InBound,lpBuffer,nNumberOfBytesToWrite,lpOverlapped,lpCompletionRoutine);
	}
	;
	UINT ReadOutEx(LPVOID lpBuffer,
		DWORD nNumberOfBytesToWrite,
		LPOVERLAPPED lpOverlapped,
		LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)const
	{
		if(OutBound==NULL){SetLastError(RPC_X_NULL_REF_POINTER);return 0;}
		return ReadFileEx(OutBound,lpBuffer,nNumberOfBytesToWrite,lpOverlapped,lpCompletionRoutine);
	};
	UINT ReadIn(LPVOID lpBuffer,
		DWORD nNumberOfBytesToWrite,
		LPDWORD lpNumberOfBytesWritten, 
		LPOVERLAPPED lpOverlapped)const
	{
		if(InBound==NULL){SetLastError(RPC_X_NULL_REF_POINTER);return 0;}
		return ReadFile(InBound,lpBuffer,nNumberOfBytesToWrite,lpNumberOfBytesWritten, lpOverlapped);
	}
	;
	UINT ReadOut(LPVOID lpBuffer,
		DWORD nNumberOfBytesToWrite,
		LPDWORD lpNumberOfBytesWritten, 
		LPOVERLAPPED lpOverlapped)const
	{
		if(OutBound==NULL){SetLastError(RPC_X_NULL_REF_POINTER);return 0;}
		return ReadFile(OutBound,lpBuffer,nNumberOfBytesToWrite,lpNumberOfBytesWritten, lpOverlapped);
	};



private:
	HANDLE InBound,OutBound;
	string  vAdapter;
	string  Adapter;
	USHORT Mac[3];
	string  Desc;
	string  IP;
	UINT type;
	string Mac_str;
};
#endif