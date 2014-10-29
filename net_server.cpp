#include "net_server.h"
#include "net_message.h"
#include "log_manager.h"
#include "time_manager.h"
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/resource.h>

SocketServer::SocketServer()
{
	lastConnectTime = new time_t[MAX_CONN + 1];
	if (lastConnectTime == NULL)
		LogManager::getInstance()->Err("Error SocketServer::SocketServer()");
	else
		memset(lastConnectTime, 0, sizeof(time_t)*(MAX_CONN + 1));

#ifdef LOST_2_MSG
	memset(m_lostRecvData, 0, sizeof(m_lostRecvData));
#endif
}

SocketServer::~SocketServer()
{
	if (lastConnectTime != NULL)
		delete[] lastConnectTime;
}

bool SocketServer::Init(int maxCon)
{
	struct sigaction sig;
	sigemptyset(&sig.sa_mask);
	sig.sa_flags = SA_RESTART;
	sig.sa_handler = SIG_IGN;
	if (sigaction(SIGPIPE, &sig, NULL) != 0)
	{
		LogManager::getInstance()->Err("sigaction error,error msg:%s", strerror(errno));
		return false;
	}
	struct rlimit sockLimit;
	sockLimit.rlim_cur = maxCon;
	sockLimit.rlim_max = maxCon;
	if (setrlimit(RLIMIT_NOFILE, &sockLimit) != 0)
	{
		LogManager::getInstance()->Err("setrlimit error:%s", strerror(errno));
		//return false;
	}

	m_epollFd = epoll_create(m_epollNum);
	if (m_epollFd < 0)
		return false;

	return true;
}

bool SocketServer::Bind(const char *ip, const char *port)
{
	m_listerSock = (int)socket(AF_INET, SOCK_STREAM, 0);
	if (m_listerSock < 0)
	{
		LogManager::getInstance()->Err("create listen socket error");
		return false;
	}
	int reuseaddr_on = 1;
	if (setsockopt(m_listerSock, SOL_SOCKET, SO_REUSEADDR, (char*)&reuseaddr_on, sizeof(reuseaddr_on)) == -1)
	{
		LogManager::getInstance()->Err("setsocketopt error");
		return false;
	}
	sockaddr_in listenAddr;
	memset(&listenAddr, 0, sizeof(listenAddr));
	listenAddr.sin_family = AF_INET;
	if (ip == NULL)
		listenAddr.sin_addr.s_addr = INADDR_ANY;
	else
		listenAddr.sin_addr.s_addr = inet_addr(ip);

	listenAddr.sin_port = htons(atoi(port));
	if (bind(m_listerSock, (struct sockaddr *)&listenAddr, sizeof(listenAddr)) < 0)
	{
		LogManager::getInstance()->Err("bind ip:%s,port:%s error", ip, port);
		return false;
	}

	if (listen(m_listerSock, 5) < 0)
	{
		LogManager::getInstance()->Err("listen error", ip, port);
		return false;
	}
	SetSock(m_listerSock);
	epoll_event ev = { 0 };
	ev.events = EPOLLIN | EPOLLET;
	ev.data.fd = m_listerSock;
	if (epoll_ctl(m_epollFd, EPOLL_CTL_ADD, m_listerSock, &ev) < 0)
		return false;
	int flag = 1;
	ioctl(m_listerSock, FIONBIO, &flag);
	return true;
}

void SocketServer::SetSock(int sock)
{
	int flag = 1;
	setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &flag, sizeof(flag));
	ioctl(sock, FIONBIO, &flag);
}

void SocketServer::OnRecv(int sock)
{
	if ((sock < 0) || (sock >= MAX_CONN))
		return;
#ifdef LOST_2_MSG
	uint8 *pLost = (uint8*)(m_lostRecvData + sock);

	if (pLost[0] == 0)
	{
		recv(sock, pLost, 2, 0);
	}
	else if (pLost[1] == 0)
	{
		recv(sock, pLost + 1, 1, 0);
	}
#endif

	NetMessage *pMsg = NULL;
	if (!m_recvMsgList.Find(sock, pMsg))
	{
		pMsg = new NetMessage;
		m_recvMsgList.Insert(sock, pMsg);
	}
	int len = 0;
	while ((len = pMsg->Recv(sock)) > 0)
	{
		if (pMsg->GetLength() > NETMSG_MAX_LEN)
		{
			m_recvMsgList.Erase(sock);
			delete pMsg;
			CloseConnect(sock);
			return;
		}
		if (pMsg->RecvDone())
		{
			m_recvMsgList.Erase(sock);
			if (!m_packageList.AddMsg(pMsg, sock))
			{
				CloseConnect(sock);
				return;
			}
			UpdateConnectTime(sock);
			pMsg = new NetMessage;
			m_recvMsgList.Insert(sock, pMsg);
		}
	}
	if (((len < 0) && (errno != EAGAIN))
		|| (len == 0))
	{
		pMsg = NULL;
		m_recvMsgList.Erase(sock, pMsg);
		delete pMsg;
		CloseConnect(sock);
	}
}

void SocketServer::OnSend(int sock)
{
	NetMessage *pMsg = NULL;
	boost::mutex::scoped_lock lk(m_sendListMutex);
	if (m_sendMsgList.Find(sock, pMsg))
	{
		pMsg->Send(sock);
		if (pMsg->SendDone())
		{
#ifdef DEBUG
			PoolString *str = pMsg->GetMsgData();
			for (int i = 0; i < (int)str->length(); i++)
			{
				cout << (int)str->at(i) << " ";
			}
			cout << endl;
#endif
			m_sendMsgList.Erase(sock);
			delete pMsg;
			epoll_event ev = { 0 };
			ev.events = EPOLLIN | EPOLLET;
			ev.data.fd = sock;
			epoll_ctl(m_epollFd, EPOLL_CTL_MOD, sock, &ev);
		}
	}
}

void SocketServer::SendMsg(int sock, NetMessage &msg, bool clearOld)
{
	if (sock < 0)
	{
		return;
	}

	msg.ReSend();

	boost::mutex::scoped_lock lk(m_sendListMutex);
	if (clearOld)
	{
		NetMessage *pMsg = NULL;
		m_sendMsgList.Erase(sock, pMsg);
		delete pMsg;
	}
	NetMessage *pMsg;
	if (m_sendMsgList.Find(sock, pMsg))
	{
		pMsg->Append(msg);
	}
	else
	{
		msg.Send(sock);
		if (msg.SendDone())
		{
#ifdef DEBUG
			PoolString *str = msg.GetMsgData();
			for (int i = 0; i < (int)str->length(); i++)
			{
				cout << (int)str->at(i) << " ";
			}
			cout << endl;
#endif		 
			return;
		}
		else if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
		{
			NetMessage *pMsg = new NetMessage;
			pMsg->Copy(msg);
			m_sendMsgList.Insert(sock, pMsg);
			epoll_event ev = { 0 };
			ev.events = EPOLLIN | EPOLLOUT | EPOLLET;
			ev.data.fd = sock;
			epoll_ctl(m_epollFd, EPOLL_CTL_MOD, sock, &ev);
		}
	}
}

void SocketServer::CloseConnect(int sock)
{
	epoll_event ev = { 0 };
	ev.data.fd = sock;
	epoll_ctl(m_epollFd, EPOLL_CTL_DEL, sock, &ev);

	m_onClose(sock);
	m_packageList.DelMsg(sock);
	close(sock);
	boost::mutex::scoped_lock lk(m_sendListMutex);
	NetMessage *pMsg = NULL;
	m_sendMsgList.Erase(sock, pMsg);
	lastConnectTime[sock] = 0;
	delete pMsg;
}

void SocketServer::UpdateConnectTime(int sock)
{
	if (sock < 0 || sock > MAX_CONN)
		return;
	lastConnectTime[sock] = TimeManager::getInstance()->GetTime();
}

void SocketServer::ClearTimeOutConnect()
{
	// TODO:PJ 该功能不可用
	//static time_t t = GetSysTime();
	//time_t curTime = GetSysTime();
	//if (curTime - t >= 100)
	//{
	//	t = curTime;
	//	COnlineUser &onlineUser = SingletonOnlineUser::instance();
	//	for (int sock = 100; sock <= MAX_CONN; sock++)
	//	{
	//		if (lastConnectTime[sock] > 0 && curTime - lastConnectTime[sock] >= 300)		// 5mins time out
	//		{
	//			if (onlineUser.GetUserBySock(sock).get() == NULL)
	//			{
	//				if (sock != mdCheckSock)
	//				{
	//					m_recvMsgList.Erase(sock);
	//					CloseConnect(sock);
	//					cout << ">>> time: " << GetDay() << " " << GetHour() << ":" << GetMinute() << "  Close sock=" << sock << endl;
	//				}
	//			}
	//		}
	//	}
	//}
}

bool SocketServer::AddEvent(int sock)
{
	epoll_event ev = { 0 };
	ev.events = EPOLLIN | EPOLLET;
	ev.data.fd = sock;
	return epoll_ctl(m_epollFd, EPOLL_CTL_ADD, sock, &ev) == 0;
}

/*#ifdef QQ
bool SocketServer::IsQQServer(uint32 ip)
{
return m_QQServerIp == ip;
}
const int MAX_QQ_MSG_LEN = 4096;
static char sRecvQQMsg[MAX_QQ_MSG_LEN];
static int sRecvBegin = 0;

void SocketServer::DealQQMsg(char *pMsg,int msgLen)
{
}

void SocketServer::OnRecvQQ(int sock)
{
if((sock < 0) || (sock >= MAX_CONN))
return;

int len = 0;
int msgLen = 0;
while((len = recv(sock,sRecvQQMsg+sRecvBegin,MAX_QQ_MSG_LEN-sRecvBegin,0)) > 0)
{
sRecvBegin += len;
while(sRecvBegin > 4)
{
memcpy(&msgLen,sRecvQQMsg,4);
msgLen = ntohl(msgLen);
if(msgLen <= sRecvBegin)
{
DealQQMsg(sRecvQQMsg,msgLen);
if(msgLen != sRecvBegin)
{
memmove(sRecvQQMsg,sRecvQQMsg+msgLen,sRecvBegin-msgLen);
}
sRecvBegin = sRecvBegin-msgLen;
}
else
{
break;
}
}
}

if (((len < 0) && (errno != EAGAIN))
|| (len == 0))
{
epoll_event ev = {0};
ev.data.fd = sock;
epoll_ctl(m_epollFd,EPOLL_CTL_DEL,sock,&ev);
close(sock);
}
}
#endif
*/

void SocketServer::DespatchEvent(int timeOut)
{
	int pollNum = 0;
	int i;
	int acceptSock;

	sockaddr addr;
	socklen_t len = sizeof(addr);

	pollNum = epoll_wait(m_epollFd, m_epollEvent, m_epollNum, timeOut);

	for (i = 0; i < pollNum; i++)
	{
		if (m_epollEvent[i].data.fd == m_listerSock)
		{
			while ((acceptSock = accept(m_listerSock, &addr, &len)) > 0)
			{
				if (acceptSock >= MAX_CONN)
				{
					close(acceptSock);
					continue;
				}
				if (acceptSock < 0)
					continue;
				if (!OnAccept(acceptSock, (sockaddr_in*)&addr))
				{
					close(acceptSock);
					continue;
				}
				SetSock(acceptSock);
				if (!AddEvent(acceptSock))
				{
					close(acceptSock);
				}
				UpdateConnectTime(acceptSock);
#ifdef LOST_2_MSG
				m_lostRecvData[acceptSock] = 0;
#endif
			}
		}
		else
		{
			if ((m_epollEvent[i].events & EPOLLIN) != 0)
			{
				/*#ifdef QQ
				sockaddr_in addr;
				socklen_t len = sizeof(addr);
				getpeername(m_epollEvent[i].data.fd, (sockaddr*)&addr,&len);
				if(IsQQServer(addr.sin_addr.s_addr))
				{
				OnRecvQQ(m_epollEvent[i].data.fd);
				}
				else
				#endif//QQ*/
				OnRecv(m_epollEvent[i].data.fd);
			}
			if ((m_epollEvent[i].events & EPOLLOUT) != 0)
			{
				OnSend(m_epollEvent[i].data.fd);
			}
			if (((m_epollEvent[i].events & EPOLLERR) != 0)
				|| ((m_epollEvent[i].events & EPOLLHUP) != 0))
			{
				NetMessage *pMsg = NULL;
				m_recvMsgList.Erase(m_epollEvent[i].data.fd, pMsg);
				delete pMsg;
				CloseConnect(m_epollEvent[i].data.fd);
			}
		}
	}
}

bool SocketServer::OnAccept(int, sockaddr_in *pAddr)
{
	//cout<<pAddr->sin_addr.s_addr<<endl;
	return true;
	//cout<<inet_ntoa(pAddr->sin_addr)<<endl;
	//return true;
	//return check_mobile(pAddr->sin_addr.s_addr) != 0;
}

void SocketServer::ObserveConnectClose(boost::function<void(int)> fun)
{
	m_onClose = fun;
}

int SocketServer::Connect(const char *ip, const char *port)
{
	return -1;
}

