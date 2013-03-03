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

	const string * getvName() const;
	const string * getName() const;
	const string * getMac() const;
	const string * getDesc() const;
	const string * getIp() const;
	UINT getType();

	void setvName(const char *);
	void setName(const char *);
	void setIp(const char *);
	void setMac(const USHORT *);
	void setDesc(const char *);
	void setType(UINT);
   // BindAdapter * WantSet() const; //use only want closehandle


	BOOL isSameMac(string) const;
	BOOL isSameMac(USHORT *) const;
	BOOL isSameIP(string) const;

	UINT OpenHandle();
	UINT OpenInBound();
	UINT OpenOutBound();

	UINT ResetHook() const;
	UINT ResetInHook() const;
	UINT ResetOutHook() const;

	UINT SetHook() const;
	UINT SetInHook() const;
	UINT SetOutHook() const;

   
	UINT CloseHandles() const;
	UINT CloseInHandle() const;
	UINT CloseOutHandle() const;


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