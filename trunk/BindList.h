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
	
	static BindList *getAllBindList();
	BindAdapter * getByMac(string mac);
	BindAdapter * getByIP(string ip);
	BindAdapter * getByName(string s);
	static uint GetError();
	uint getSize();
	vector<BindAdapter> *getLists();
	private:
	static BindList _self;
	static uint err;
	vector <BindAdapter> Adapters;
};
#endif