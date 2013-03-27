#ifndef _H_CLS_BINDAPT
#define  _H_CLS_BINDAPT
#include <string>
#include "main.h"
using namespace std;

class CBindAdapter
{
public:
	CBindAdapter(const wchar_t *vname,const wchar_t *name);
	static UINT StrMacConv(wstring,USHORT *) ;
	static UINT MacToStr(USHORT *,wchar_t *) ;

	const wstring * getvName() const{return &m_svAdapter;};
	const wstring * getName() const{return &m_sAdapter;};
	const wstring * getMac() const{return &m_sMac;};
	const wstring * getDesc() const{return &m_sDesc;};
	const wstring * getIp() const{return &m_sIP;};
	UINT getType() const {return m_nType;};

	void setvName(const wchar_t * s){m_svAdapter=s;};
	void setName(const wchar_t *s){m_sAdapter=s;};
	void setIp(const wchar_t *s){m_sIP=s;};
	void setMac(const USHORT *);
	void setDesc(const wchar_t *s){m_sDesc=s ;};
	void setType(UINT s){m_nType=s ;};

	CBindAdapter * BeginRequest() const{return const_cast<CBindAdapter *>(this);}; ///< deprecated


	BOOL isSameMac(wstring s) const{return m_sMac==s; };
	BOOL isSameMac(USHORT *) const;
	BOOL isSameIP(wstring s) const{ return m_sIP==s;};

	UINT OpenHandles() const;
	UINT OpenInBound() const;
	UINT OpenOutBound() const;

	UINT ResetHook() const;
	UINT ResetInHook() const;
	UINT ResetOutHook() const;

	UINT SetInHook(PPKT_REDIR_FILTER_ENTRY pPktRedirFilterList, 
		ULONG nPktRedirFilterListEntryCount ) const;
	UINT SetOutHook(PPKT_REDIR_FILTER_ENTRY pPktRedirFilterList, 
		ULONG nPktRedirFilterListEntryCount ) const;

   
	UINT CloseHandles() const;
	UINT CloseInHandle() const;
	UINT CloseOutHandle() const;


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
	mutable HANDLE m_hInBound,m_hOutBound;
	wstring  m_svAdapter;
	wstring  m_sAdapter;
	USHORT m_Mac[3];
	wstring  m_sDesc;
	wstring  m_sIP;
	UINT m_nType;
	wstring m_sMac;
};
#endif