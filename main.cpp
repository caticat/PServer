#include "type_define.h"
#include "config.h"
#include "server.h"

#include "time_manager.h"
#include "log_manager.h"

PServer* g_pServer;

int main()
{
	TimeManager::getInstance()->Sync();
	if (!Config::getInstance()->load()) // ���������ļ�
		return 1;
	if (!Config::getInstance()->initDb()) // �������ݿ�
		return 2;

	// ����������
	PServer* pServer = new PServer;
	if (pServer->Init()) // ��ʼ��
	{
		g_pServer = pServer;
		pServer->Run(); // �����߳�
	}

	// ɾ������������
	delete pServer;
	return 0;
}