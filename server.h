#pragma once

/*
	��������
*/

#include <boost/thread.hpp>

class PServer
{
public:
	PServer() : m_thread(NULL), m_isRunning(true) {}
	bool Init();
	void Run();

public:
	bool IsRunning() { return m_isRunning; } // �������Ƿ���������
	void Stop() { m_isRunning = false; } // ֹͣ������

private:
	void PackageDeal(); // ������Ϣ����
	void Timer(); // ��ʱ��
	void SignalDeal(); // �źŴ���
	void Join(); // �̵߳ȴ�

private:
	boost::thread** m_thread; // �̹߳�������
	bool m_isRunning; // �������Ƿ���������
};
