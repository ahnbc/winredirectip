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

	const string * getvName() const{return &m_svAdapter;};
	const string * getName() const{return &m_sAdapter;};
	const string * getMac() const{return &m_sMac;};
	const string * getDesc() const{return &m_sDesc;};
	const string * getIp() const{return &m_sIP;};
	UINT getType() const {return m_nType;};

	void setvName(const char * s){m_svAdapter=s;};
	void setName(const char *s){m_sAdapter=s;};
	void setIp(const char *s){m_sIP=s;};
	void setMac(const USHORT *);
	void setDesc(const char *s){m_sDesc=s ;};
	void setType(UINT s){m_nType=s ;};

	BindAdapter * BeginRequest() const{return const_cast<BindAdapter *>(this);}; 


	BOOL isSameMac(string s) const{return m_sMac==s; };
	BOOL isSameMac(USHORT *) const;
	BOOL isSameIP(string s) const{ return m_sIP==s;};

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


	BOOL isOutHandleOpen() const {return m_hOutBound!=NULL;};
	BOOL isInHandleOpen() const {return m_hInBound!=NULL;};


	UINT WriteInEx(LPCVOID lpBuffer,
		 DWORD nNumberOfBytesToWrite,
		LPOVERLAPPED lpOverlapped,
		LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)const
	{
		if(m_hInBound==NULL){SetLastError(RPC_X_NULL_REF_POINTER);return 0;}
		return WriteFileEx(m_hInBound,lpBuffer,nNumberOfBytesToWrite,lpOverlapped,lpCompletionRoutine);
	}
	;
	UINT WriteOutEx(LPCVOID lpBuffer,
		DWORD nNumberOfBytesToWrite,
		LPOVERLAPPED lpOverlapped,
		LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)const
	{
		if(m_hOutBound==NULL){SetLastError(RPC_X_NULL_REF_POINTER);return 0;}
		return WriteFileEx(m_hOutBound,lpBuffer,nNumberOfBytesToWrite,lpOverlapped,lpCompletionRoutine);
	};
	UINT WriteIn(LPCVOID lpBuffer,
		DWORD nNumberOfBytesToWrite,
		LPDWORD lpNumberOfBytesWritten, 
		LPOVERLAPPED lpOverlapped)const
	{
		if(m_hInBound==NULL){SetLastError(RPC_X_NULL_REF_POINTER);return 0;}
		return WriteFile(m_hInBound,lpBuffer,nNumberOfBytesToWrite,lpNumberOfBytesWritten, lpOverlapped);
	}
	;
	UINT WriteOut(LPCVOID lpBuffer,
		DWORD nNumberOfBytesToWrite,
		LPDWORD lpNumberOfBytesWritten, 
		LPOVERLAPPED lpOverlapped)const
	{
		if(m_hOutBound==NULL){SetLastError(RPC_X_NULL_REF_POINTER);return 0;}
		return WriteFile(m_hOutBound,lpBuffer,nNumberOfBytesToWrite,lpNumberOfBytesWritten, lpOverlapped);
	};


	UINT ReadInEx(LPVOID lpBuffer,
		DWORD nNumberOfBytesToWrite,
		LPOVERLAPPED lpOverlapped,
		LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)const
	{
		if(m_hInBound==NULL){SetLastError(RPC_X_NULL_REF_POINTER);return 0;}
		return ReadFileEx(m_hInBound,lpBuffer,nNumberOfBytesToWrite,lpOverlapped,lpCompletionRoutine);
	}
	;
	UINT ReadOutEx(LPVOID lpBuffer,
		DWORD nNumberOfBytesToWrite,
		LPOVERLAPPED lpOverlapped,
		LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)const
	{
		if(m_hOutBound==NULL){SetLastError(RPC_X_NULL_REF_POINTER);return 0;}
		return ReadFileEx(m_hOutBound,lpBuffer,nNumberOfBytesToWrite,lpOverlapped,lpCompletionRoutine);
	};
	UINT ReadIn(LPVOID lpBuffer,
		DWORD nNumberOfBytesToWrite,
		LPDWORD lpNumberOfBytesWritten, 
		LPOVERLAPPED lpOverlapped)const
	{
		if(m_hInBound==NULL){SetLastError(RPC_X_NULL_REF_POINTER);return 0;}
		return ReadFile(m_hInBound,lpBuffer,nNumberOfBytesToWrite,lpNumberOfBytesWritten, lpOverlapped);
	}
	;
	UINT ReadOut(LPVOID lpBuffer,
		DWORD nNumberOfBytesToWrite,
		LPDWORD lpNumberOfBytesWritten, 
		LPOVERLAPPED lpOverlapped)const
	{
		if(m_hOutBound==NULL){SetLastError(RPC_X_NULL_REF_POINTER);return 0;}
		return ReadFile(m_hOutBound,lpBuffer,nNumberOfBytesToWrite,lpNumberOfBytesWritten, lpOverlapped);
	};



private:
	HANDLE m_hInBound,m_hOutBound;
	string  m_svAdapter;
	string  m_sAdapter;
	USHORT m_Mac[3];
	string  m_sDesc;
	string  m_sIP;
	UINT m_nType;
	string m_sMac;
};
#endif