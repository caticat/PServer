#pragma once

/*
	网络总处理
*/

#include "macro_define.h"
#include "const_define.h"
#include "hash_table.h"
#include "net_packlist.h"
#include <sys/epoll.h>
#include <boost/function.hpp>
#include <boost/thread/mutex.hpp>

class NetMessage;
struct sockaddr_in;

class SocketServer
{
	SINGTONE_GETINSTANCE(SocketServer);
public:
	SocketServer();
	virtual ~SocketServer();
	bool Init(int maxConnect);
	bool Bind(const char* ip, const char* port);
	void DespatchEvent(int timeOut);

	// 发送消息
	void SendMsg(int sock, NetMessage& msg, bool clearOld = false);

	void ObserveConnectClose(boost::function<void(int)>);

	// 连接远程服务器
	int Connect(const char *ip, const char *port);

	void SetSock(int sock);

	//关闭连接
	void CloseConnect(int sock);

	NetMessage *GetPackage(int &sock)
	{
		return m_packageList.GetMsg(sock);
	}

	bool AddEvent(int sock);
	void ClearTimeOutConnect();

protected:
	//继承者可以实现此函数，如果返回false，框架会不允许此连接
	virtual bool OnAccept(int, sockaddr_in*);

	virtual void OnRecv(int sock);
	virtual void OnSend(int sock);

private:
	void UpdateConnectTime(int sock);
	int m_listerSock;
	int m_epollFd;
	static const int m_epollNum = MAX_CONN;
	epoll_event m_epollEvent[m_epollNum];
#ifdef LOST_2_MSG
	uint16  m_lostRecvData[MAX_CONN];
#endif

	CHashTable<int, NetMessage*> m_sendMsgList;
	boost::mutex m_sendListMutex;
	CHashTable<int, NetMessage*> m_recvMsgList;
	CPackageList m_packageList;
	boost::function<void(int)> m_onClose;
	time_t *lastConnectTime;
};
