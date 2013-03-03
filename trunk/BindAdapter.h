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
	BOOL isSameMac(string) const;
	BOOL isSameMac(USHORT *) const;
	BOOL isSameIP(string) const;
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