#include "net_message.h"
#include "log_manager.h"
#include <sys/socket.h>
#include <boost/pool/singleton_pool.hpp>

typedef boost::singleton_pool<NetMessage, sizeof(NetMessage)> MsgPool;

NetMessage::NetMessage() : m_readPos(NETMSG_HEAD_LEN), m_recvLen(0), m_sendBegin(0)
{
	m_msg.resize(NETMSG_HEAD_LEN);
	SetLength();
}

uint16 NetMessage::GetLength()
{
	return *(uint16*)(m_msg.c_str()) + NETMSG_HEAD_LEN;
}

void NetMessage::SetLength()
{
	*(uint16*)(m_msg.c_str()) = (uint16)m_msg.length() - NETMSG_HEAD_LEN;
}

std::string* NetMessage::GetData()
{
	return &m_msg;
}

void NetMessage::SetType(uint16 type)
{
	*(uint16*)(m_msg.c_str() + NETMSG_TYPE_BEGIN) = type;
}

uint16 NetMessage::GetType()
{
	return *(uint16*)(m_msg.c_str() + NETMSG_TYPE_BEGIN);
}

void NetMessage::ReWrite()
{
	m_sendBegin = 0;
	m_msg.clear();
	m_msg.resize(NETMSG_HEAD_LEN);
}

void NetMessage::ReRead()
{
	m_readPos = NETMSG_HEAD_LEN;
}

void NetMessage::Copy(NetMessage& msg)
{
	m_msg.clear();
	char *data = (char*)(msg.m_msg.c_str() + msg.m_sendBegin);
	int dataLen = msg.m_msg.length() - msg.m_sendBegin;
	m_msg.insert(0, data, dataLen);
	m_sendBegin = 0;
}

void NetMessage::Write(uint16 begin, void* pData, uint16 lenth)
{
	if (begin + lenth > (int)m_msg.length())
		return;
	char *p = (char*)m_msg.c_str() + begin;
	memcpy(p, pData, lenth);
}

void NetMessage::SetData(void* pData, uint16 length)
{
	if (pData == NULL)
		return;
	m_msg.clear();
	m_msg.insert(0, (char*)pData, length);
}

void* NetMessage::operator new(size_t)
{
	return MsgPool::malloc();
}

void NetMessage::operator delete(void* p)
{
	MsgPool::free(p);
}

int NetMessage::Recv(int sock)
{
	using std::cout; using std::endl;
	char *msg = (char*)m_msg.c_str() + m_recvLen;
	int len = 0;
	cout << "1 m_recvLen:" << m_recvLen << ",sock:" << sock << endl;
	if (m_recvLen < NETMSG_HEAD_LEN)
	{
		len = recv(sock, msg, NETMSG_HEAD_LEN - m_recvLen, 0);
		cout << "2 len:" << len << endl;
		if (len > 0)
			m_recvLen += len;
		if (m_recvLen == NETMSG_HEAD_LEN)
		{
			if (GetLength() > NETMSG_MAX_LEN)
				return 0;
			m_msg.resize(GetLength());
		}
		cout << "3 m_recvLen:" << m_recvLen << endl;
		return len;
	}
	if (GetLength() < m_recvLen)
		return 0;
	msg = (char*)m_msg.c_str() + m_recvLen;

	len = recv(sock, msg, GetLength() - m_recvLen, 0);
	cout << "4 len:" << len << endl;
	if (len > 0)
		m_recvLen += len;
	cout << "5 m_recvLen:" << m_recvLen << endl;
	return len;
}

bool NetMessage::RecvDone()
{
	return (GetLength() == m_recvLen);
}

int NetMessage::Send(int sock)
{
	char *msg = (char*)m_msg.c_str() + m_sendBegin;
	int len = send(sock, msg, (int)(m_msg.length() - m_sendBegin), 0);
	if (len > 0)
		m_sendBegin += len;
	return len;
}

bool NetMessage::SendDone()
{
	bool flag = (m_sendBegin >= m_msg.length());
	if (flag)
		m_sendBegin = 0;
	return flag;
}

void NetMessage::Write(void* pData, uint16 length)
{
	m_msg.insert(m_msg.length(), (char*)pData, length);
	SetLength();
}

void NetMessage::Append(NetMessage& msg)
{
	if (m_msg.length() >= NETMSG_MAX_LEN)
	{
		LogManager::getInstance()->Err("net msg too long.type:%d", GetType());
		return;
	}
	char *data = (char*)(msg.m_msg.c_str() + msg.m_sendBegin);
	uint16 dataLen = msg.m_msg.length() - msg.m_sendBegin;
	m_msg.erase(0, m_sendBegin);
	m_sendBegin = 0;
	m_msg.append(data, dataLen);
}

void NetMessage::Read(void* pData, uint16 length)
{
	if (m_readPos + length > (int)m_msg.length())
	{
		LogManager::getInstance()->Err("read over overflow.type:%d", GetType());
		return;
	}
	char *p = (char*)m_msg.c_str() + m_readPos;
	memcpy(pData, p, length);
	m_readPos += length;
}

void NetMessage::ReadString(std::string& str)
{
	uint16 len = 0;
	Read(&len, sizeof(len));
	if (len <= 0)
		return;
	if (m_readPos + len > (int)m_msg.length())
	{
		LogManager::getInstance()->Err("read string over overflow.type:%d", GetType());
		return;
	}
	char *p = (char*)(m_msg.c_str() + m_readPos);
	str.clear();
	str.insert(0, p, len);
	m_readPos += len;
}
