#pragma once

/*
	主程序类
*/

#include <boost/thread.hpp>

class PServer
{
public:
	PServer() : m_thread(NULL), m_isRunning(true) {}
	bool Init();
	void Run();

public:
	bool IsRunning() { return m_isRunning; } // 服务器是否正常运行
	void Stop() { m_isRunning = false; } // 停止服务器

private:
	void PackageDeal(); // 网络消息处理
	void Timer(); // 定时器
	void SignalDeal(); // 信号处理
	void Join(); // 线程等待

private:
	boost::thread** m_thread; // 线程管理数组
	bool m_isRunning; // 服务器是否正在运行
};
