#pragma once

/*
	消息处理类
*/

class SocketServer;
class NetMessage;

class PackageDeal
{
public:
	PackageDeal();

private:
	void OnSockClose(int); // 连接断开
	void PTL_Test(int, NetMessage*); // 测试协议

private:
	SocketServer& m_socketServer;
};
