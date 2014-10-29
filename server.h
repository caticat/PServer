#pragma once

/*
	��������
*/

#include "net_server.h"
#include "net_despatch.h"
#include "net_packdeal.h"
#include <boost/thread.hpp>

class PServer
{
public:
	PServer() : m_thread(NULL), m_isRunning(true), m_socketServer(*SocketServer::getInstance()), m_despatch(*DespatchCommand::getInstance()){}
	bool Init();
	void Run();

public:
	bool IsRunning() { return m_isRunning; } // �������Ƿ���������
	void Stop() { m_isRunning = false; } // ֹͣ������

private:
	void PackDeal(); // ������Ϣ����
	void Timer(); // ��ʱ��
	void SignalDeal(); // �źŴ���
	void Join(); // �̵߳ȴ�

private:
	boost::thread** m_thread; // �̹߳�������
	bool m_isRunning; // �������Ƿ���������

	PackageDeal m_packDeal; // ���Ժ󼴿ɣ���������Э����Զ�����
	SocketServer& m_socketServer;
	DespatchCommand& m_despatch;
};
