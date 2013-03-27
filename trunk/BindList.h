/************************************************************************/
/**
 * @file: BindList
 * @author:nowind
 * @email:nowind01@gmail.com
 * @date:2013/03/27
 * @version: 0.1.2
 * 
 * 
/************************************************************************/


#ifndef _H_CLS_BINDLIS
#define  _H_CLS_CBindList
#include "main.h"
#include <string>
#include <vector>
#include "BindAdapter.h"
using namespace std;
/**
 * @class: 用来获取系统支持的所有网卡适配器
 * @description:
 *		直接调用 @see GetAllCBindList 获取列表
 *		调用 @see GetSize 获取列表大小
 *		调用 @see getByMac @see getByIP @see getByName 进行查找
 *
 **/
class CBindList
{

public:
	static BOOL initFlag;

public:
	static const CBindList *GetAllBindList();
	const CBindAdapter * getByMac(wstring mac)const ;
	const CBindAdapter * getByIP(wstring ip) const;
	const CBindAdapter * getByName(wstring s) const;
	uint GetSize();
	const vector<CBindAdapter> *getLists() const;

public:
	static uint GetError();

private:
	static CBindList s_self;
	static  uint s_err;

private:
	vector <CBindAdapter> m_Adapters;
};
#endif