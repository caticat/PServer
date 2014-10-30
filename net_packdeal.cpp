#include "net_packdeal.h"
#include "net_server.h"
#include "net_despatch.h"
#include "net_protocol.h"
#include "net_message.h"
#include "log_manager.h"
#include <boost/bind.hpp>

PackageDeal::PackageDeal() : m_socketServer(*SocketServer::getInstance())
{
	DespatchCommand& despatch = *DespatchCommand::getInstance();

	SCommand cmdFun[] = 
	{
		//{ PTL_TEST , boost::bind(&PackageDeal::PTL_Test,this,_1,_2)},
		PTL_BAND(PTL_TEST, PTL_Test),
	};
	m_socketServer.ObserveConnectClose(boost::bind(&PackageDeal::OnSockClose, this, _1));
	despatch.AddCommandDeal(cmdFun, sizeof(cmdFun) / sizeof(SCommand));
}

void PackageDeal::OnSockClose(int sock)
{
	LogManager::getInstance()->Log("sock:%d is closed.", sock);
}

void PackageDeal::PTL_Test(int sock, NetMessage* pMsg)
{
	LogManager::getInstance()->Log("收到消息了！sock:%d", sock);
	CONFIRM(pMsg);

	uint8 u8 = 0;
	uint16 u16 = 0;
	uint32 u32 = 0;
	uint64 u64 = 0;
	std::string str = "";
	uint64 u64empty = 0;
	*pMsg >> u8 >> u16 >> u32 >> u64 >> str;// >> u64empty;
	std::cout << "u8:" << (int)u8 << ",u16:" << (int)u16 << ",u32:" << (int)u32 << ",u64:" << u64 << ",str:" << str << ",empty:" << (int)u64empty << std::endl;

	NetMessage msg;
	msg.SetType(PTL_TEST);
	msg << (uint8)111 << (uint16)65530 << (uint32)200000001 << (uint64)1234567890123 << "abcdefg";
	m_socketServer.SendMsg(sock, msg);
}
