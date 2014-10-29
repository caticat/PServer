#pragma once

/*
	消息管理类
*/

#include <utility>
#include <list>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>
#include <boost/thread/mutex.hpp>

class NetMessage;
typedef std::pair<int, NetMessage*> MsgNode;

class CPackageList
{
public:
	CPackageList() :m_semphone(0)
	{
	}

	NetMessage *GetMsg(int &sock);
	bool AddMsg(NetMessage *pMsg, int sock);
	void DelMsg(int sock);
private:
	std::list<MsgNode> m_msgList;
	boost::interprocess::interprocess_semaphore m_semphone;
	boost::mutex    m_listMutex;
};
