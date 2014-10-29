#include "net_packlist.h"
#include "net_message.h"
#include <algorithm>
#include <boost/bind.hpp>

NetMessage *CPackageList::GetMsg(int &sock)
{
	NetMessage *pMsg = NULL;
	try
	{
		//if(m_semphone.timed_wait(const boost::posix_time::ptime & abs_time))
		if (m_semphone.try_wait())
		{
			boost::mutex::scoped_lock lk(m_listMutex);
			if (m_msgList.size() > 0)
			{
				pMsg = (*m_msgList.begin()).second;
				sock = (*m_msgList.begin()).first;
				m_msgList.pop_front();
			}
		}
		else
		{
			usleep(50);
		}
	}
	catch (...)
	{
	}
	return pMsg;
}

bool MsgEqual(const MsgNode &node, int sock)
{
	return node.first == sock;
}

bool CPackageList::AddMsg(NetMessage *pMsg, int sock)
{
	boost::mutex::scoped_lock lk(m_listMutex);
	int num = std::count_if(m_msgList.begin(), m_msgList.end(), boost::bind(MsgEqual, _1, sock));
	if (num > 200)
	{
		delete pMsg;
		return false;
	}
	m_msgList.push_back(std::make_pair(sock, pMsg));
	m_semphone.post();
	return true;
}

void CPackageList::DelMsg(int sock)
{
	boost::mutex::scoped_lock lk(m_listMutex);
	std::list<MsgNode>::iterator i = m_msgList.begin();
	std::list<MsgNode>::iterator next = i;
	for (; i != m_msgList.end();)
	{
		next = i;
		next++;
		if (i->first == sock)
		{
			delete i->second;
			m_msgList.erase(i);
		}
		i = next;
		//pMsg = (*m_msgList.begin()).second;
		//sock = (*m_msgList.begin()).first;
		//m_msgList.pop_front();
	}
}
