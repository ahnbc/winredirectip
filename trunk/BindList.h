#ifndef _H_CLS_BINDLIS
#define  _H_CLS_BINDLIST
#include "main.h"
#include <string>
#include <vector>
#include "BindAdapter.h"
using namespace std;
class BindList
{
	public:
	static BOOL initFlag;
	static  BindList *getAllBindList();
	const BindAdapter * getByMac(wstring mac)const ;
	const BindAdapter * getByIP(wstring ip) const;
	const BindAdapter * getByName(wstring s) const;
	static uint GetError();
	uint getSize();
	const vector<BindAdapter> *getLists() const;
	private:
	
	static BindList s_self;
	static uint s_err;
	vector <BindAdapter> m_Adapters;
};
#endif