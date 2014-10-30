#pragma once

/*
	ÍøÂçÏûÏ¢Àà
*/

#include "type_define.h"
#include "const_define.h"
#include <string.h>

class NetMessage
{
public:
	NetMessage();

	uint16 GetLength();
	void SetLength();
	void SetType(uint16 type);
	uint16 GetType();
	void ReWrite();
	void ReRead();

	std::string* GetData();
	void Copy(NetMessage& msg);

	void Write(void* pData, uint16 length);
	void Write(uint16 begin, void* pData, uint16 lenth);
	void SetData(void* pData, uint16 length);
	void Append(NetMessage& msg);

	int Recv(int sock);
	bool RecvDone();
	int Send(int sock);
	bool SendDone();

	void* operator new(size_t);
	void operator delete(void*);

	template<typename T>
	NetMessage& operator <<(T val)
	{
		Write(&val, sizeof(val));
		return *this;
	}

	template<typename T>
	NetMessage& operator >>(T& val)
	{
		Read(&val, sizeof(val));
		return *this;
	}

	void ReSend()
	{
		m_sendBegin = 0;
	}
	uint16 GetSendBegin()
	{
		return m_sendBegin;
	}

private:
	void Read(void* pData, uint16 length);
	void ReadString(std::string& str);

private:
	uint16 m_readPos;
	std::string m_msg;
	uint16 m_recvLen;
	uint16 m_sendBegin;
};

template<>
inline NetMessage& NetMessage::operator << <std::string>(std::string val)
{
	uint16 length = (uint16)val.length();
	Write(&length, sizeof(length));
	if (length > 0)
		Write((void*)val.c_str(), length);
	return *this;
}

template<>
inline NetMessage& NetMessage::operator << <const char*> (const char* val)
{
	uint16 length = strlen(val);
	Write(&length, sizeof(length));
	if (length > 0)
		Write((void*)val, length);
	return *this;
}

template<>
inline NetMessage& NetMessage::operator << <char*> (char* val)
{
	uint16 length = (uint16)strlen(val);
	Write(&length, sizeof(length));
	if (length > 0)
		Write((void*)val, length);
	return *this;
}

template<>
inline NetMessage& NetMessage::operator >> <std::string> (std::string& val)
{
	ReadString(val);
	val.resize(val.size());
	return *this;
}

template<>
inline NetMessage& NetMessage::operator << <NetMessage> (NetMessage val)
{
	m_msg.append(val.m_msg.substr(NETMSG_HEAD_LEN));
	SetLength();
	return *this;
}

template<>
inline NetMessage &NetMessage::operator >> <NetMessage> (NetMessage& val)
{
	val.m_msg.append(m_msg.substr(NETMSG_HEAD_LEN));
	val.SetLength();
	return *this;
}

//template<> inline CNetMessage &CNetMessage::operator << <string>(string val)
//{
//	uint16 len = (uint16)val.size();
//	string str;
//	str.resize(len * 2);
//#ifndef HAIWAI
//	len = GbkToUnicode((char*)str.c_str(), len * 2, (char*)val.c_str(), len);
//#else
//	len = UTF8ToUnicode((char*)str.c_str(), len * 2, (char*)val.c_str(), len);
//#endif
//	WriteData(&len, sizeof(len));
//	if (len > 0)
//	{
//		WriteData((void*)str.c_str(), len);
//	}
//	return *this;
//}

//template<> inline CNetMessage &CNetMessage::operator>> <string> (string &val)
//{
//	string str;
//	ReadString(str);
//
//	uint16 len = (uint16)str.size();
//#ifndef HAIWAI
//	val.resize(len);
//	len = UnicodeToGbk((char*)val.c_str(), len, (char*)str.c_str(), str.size());
//#else
//	len *= 2;
//	val.resize(len);
//	len = UnicodeToUTF8((char*)val.c_str(), len, (char*)str.c_str(), str.size());
//#endif
//	val.resize(len);
//	return *this;
//}
