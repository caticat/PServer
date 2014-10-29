#pragma once

/*
	π§æﬂ¿‡
*/

#include "log_manager.h"
#include <string>
#include <iostream>
#include <sys/time.h>

class TimePrint
{
public:
	TimePrint(std::string str, int sock = 0)
	{
		gettimeofday(&m_time, NULL);
		m_str = str;
		m_sock = sock;
	}
	~TimePrint()
	{
		timeval t;
		gettimeofday(&t, NULL);
		int usec = 1000000 * (t.tv_sec - m_time.tv_sec) + t.tv_usec - m_time.tv_usec;
		if (usec >= 10000)
		{
			if (m_sock == 0)
				LogManager::getInstance()->Warn("Function %s deal too long: %lf s", m_str.c_str(), (usec / 1000000.0));
			else
				LogManager::getInstance()->Warn("Function %s deal too long: %lf s, sock: %d", m_str.c_str(), (usec / 1000000.0), m_sock);
		}
	}
private:
	std::string m_str;
	timeval m_time;
	int m_sock;
};
