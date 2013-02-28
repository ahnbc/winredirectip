#ifndef _H_CLS_BINDAPT
#define  _H_CLS_BINDAPT
#include <string>
#include "main.h"
using namespace std;

class BindAdapter
{
public:
	BindAdapter(const char *vname,const char *name);
	static UINT strMacConv(string,USHORT *);
	static UINT MactoStr(USHORT *,char *);

	const string * getvName();
	const string * getName();
	const string * getMac();
	const string * getDesc();
	const string * getIp();
	UINT getType();

	void setvName(const char *);
	void setName(const char *);
	void setIp(const char *);
	void setMac(const USHORT *);
	void setDesc(const char *);
	void setType(UINT);
	BOOL isSameMac(string);
	BOOL isSameMac(USHORT *);
	BOOL isSameIP(string);
private:
	string  vAdapter;
	string  Adapter;
	USHORT Mac[3];
	string  Desc;
	string  IP;
	UINT type;
	string Mac_str;
};
#endif