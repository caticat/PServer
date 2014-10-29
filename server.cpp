#include "server.h"

#include "const_define.h"
#include "macro_define.h"
#include "time_manager.h"
#include "log_manager.h"
#include "psignal.h"
#include "config.h"
#include "net_message.h"

bool PServer::Init()
{
	if (m_thread == NULL)
		m_thread = new boost::thread*[THREAD_NUM];
	m_thread[THREAD_PACKAGEDEAL] = new boost::thread(boost::bind(&PServer::PackDeal, this));
	m_thread[THREAD_TIMER] = new boost::thread(boost::bind(&PServer::Timer, this));
	m_thread[THREAD_SIGNAL] = new boost::thread(boost::bind(&PServer::SignalDeal, this));

	if (!m_socketServer.Init(MAX_CONN))
		return false;
	if (!m_socketServer.Bind(NULL, Config::getInstance()->port.c_str()))
		return false;

	return true;
}

void PServer::Run()
{
	while (IsRunning())
	{
		m_socketServer.DespatchEvent(EPOLL_WAIT_TIMEOUT);
	}
	Join();
}

void PServer::Join()
{
	for (int i = 0; i < THREAD_NUM; ++i)
	{
		m_thread[i]->join();
	}
	RELEASE_ARR(m_thread);
}

void PServer::PackDeal()
{
	// 网络消息处理
	int sock;
	NetMessage* pMsg;
	while (IsRunning())
	{
		pMsg = m_socketServer.GetPackage(sock);
		if (pMsg != NULL)
		{
			if (!m_despatch.Despatch(sock, pMsg))
				LogManager::getInstance()->Err("NetMessage type error. sock:%d,type:%d",sock,pMsg->GetType());
			delete pMsg;
		}
	}
}

void PServer::Timer()
{
	// 主定时器
	time_t lastCheckTime = TimeManager::getInstance()->GetTime();
	time_t curTime = 0;
	while (IsRunning())
	{
		curTime = TimeManager::getInstance()->GetTime();
		if ((curTime - lastCheckTime) > TIMER_TIMEOUT)
		{
			lastCheckTime = curTime;
			LogManager::getInstance()->Log("主定时器调用了！");
		}
		sleep(SLEEP_TIMEOUT);
	}
}

void PServer::SignalDeal()
{
	// 信号处理
	PSignal::getInstance()->Regist();

	while (IsRunning())
	{
		sleep(SLEEP_TIMEOUT);
		TimeManager::getInstance()->Sync(); // 更新系统时间
	}
}
