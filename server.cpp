#include "server.h"

#include "const_define.h"
#include "macro_define.h"
#include "time_manager.h"
#include "log_manager.h"
#include "psignal.h"

bool PServer::Init()
{
	if (m_thread == NULL)
		m_thread = new boost::thread*[THREAD_NUM];
	m_thread[THREAD_PACKAGEDEAL] = new boost::thread(boost::bind(&PServer::PackageDeal, this));
	m_thread[THREAD_TIMER] = new boost::thread(boost::bind(&PServer::Timer, this));
	m_thread[THREAD_SIGNAL] = new boost::thread(boost::bind(&PServer::SignalDeal, this));

	return true;
}

void PServer::Run()
{
	while (IsRunning())
	{
		sleep(SLEEP_TIMEOUT); // TODO:PJ 这个要删掉，epoll会有的
		//m_socketServer.DespatchEvent(1000);
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

void PServer::PackageDeal()
{
	// 网络消息处理
	sleep(SLEEP_TIMEOUT); // TODO:PJ 这里后面要删掉
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
