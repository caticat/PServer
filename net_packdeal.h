#pragma once

/*
	��Ϣ������
*/

class SocketServer;
class NetMessage;

class PackageDeal
{
public:
	PackageDeal();

private:
	void OnSockClose(int); // ���ӶϿ�
	void PTL_Test(int, NetMessage*); // ����Э��

private:
	SocketServer& m_socketServer;
};
