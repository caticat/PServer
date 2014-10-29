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
		{ PTL_TEST , boost::bind(&PackageDeal::PTL_Test,this,_1,_2)},
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

	NetMessage msg;
}
