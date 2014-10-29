#include "net_despatch.h"
#include "net_message.h"
#include "utility.h"

bool DespatchCommand::AddCommand(uint16 comType, CommandFun fun)
{
	/*if(m_funMap.find(comType) != m_funMap.end())
	{
	assert(1);
	return false;
	}*/
	//m_funMap[comType] = fun;
	m_funMap.Insert(comType, fun);
	return true;
}

bool DespatchCommand::Despatch(int sock, NetMessage *pMsg)
{
	if (pMsg == NULL)
		return false;
	CommandFun fun;
	if (!m_funMap.Find(pMsg->GetType(), fun))
	{
		return false;
	}

	/*	string str = "";
	if(pMsg->GetType() == MSG_ARENA)
	str = "¾º¼¼³¡";
	else if(pMsg->GetType() == PRO_BATTLE)
	str = "Õ½¶·";
	if(str.length() > 0)
	{
	TimePrint t(str,sock);
	fun(pMsg,sock);
	}
	else
	*/
	char buf[32];
	snprintf(buf, sizeof(buf), "%d", (int)pMsg->GetType());
	TimePrint t(buf, sock);
	fun(sock, pMsg);
	return true;
}

void DespatchCommand::AddCommandDeal(SCommand *pBegin, int num)
{
	for (int i = 0; i < num; i++)
	{
		AddCommand(pBegin->comType, pBegin->comFun);
		pBegin++;
	}
}
