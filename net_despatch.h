#pragma once

/*
	消息分发
*/

#include "macro_define.h"
#include "type_define.h"
#include "hash_table.h"
#include <boost/function.hpp>
#include <list>

class NetMessage;

typedef boost::function<void(int, NetMessage*)> CommandFun;

struct  SCommand
{
	uint16      comType;
	CommandFun  comFun;
};

class DespatchCommand
{
	SINGTONE_GETINSTANCE(DespatchCommand);
public:
	DespatchCommand() :m_funMap(512)
	{
	}
	void AddCommandDeal(SCommand *pBegin, int num);
	bool Despatch(int sock, NetMessage*);
private:
	bool AddCommand(uint16 comType, CommandFun);
	CHashTable<int, CommandFun> m_funMap;
};
