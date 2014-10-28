#include "log_manager.h"
#include "config.h"
#include <stdarg.h>

LogManager::LogManager()
{
	Config* pConfig = Config::getInstance();
	// 初始化日志
	{
		m_logs[LOG_NORMAL] = log_t(new CPLog);
		m_logs[LOG_NORMAL]->init(pConfig->logPath, "NORMAL", ".txt", true, true);
	}

	{
		m_logs[LOG_WARN] = log_t(new CPLog);
		m_logs[LOG_WARN]->init(pConfig->logPath, "WARNING", ".txt", true, true);
	}

	{
		m_logs[LOG_ERROR] = log_t(new CPLog);
		m_logs[LOG_ERROR]->init(pConfig->logPath, "ERROR", ".txt", true, true);
	}

	{
		m_logs[LOG_ALL] = log_t(new CPLog);
		m_logs[LOG_ALL]->init(pConfig->logPath, "ALL", ".txt", true, false);
	}
}

void LogManager::Log(const char* fmt, ...)
{
	va_list vl;
	va_start(vl, fmt);
	DoLog(LOG_NORMAL, fmt, vl);
	All("[NORMAL]", fmt, vl);
	va_end(vl);
}

void LogManager::Warn(const char* fmt, ...)
{
	va_list vl;
	va_start(vl, fmt);
	DoLog(LOG_WARN, fmt, vl);
	All("[WARN]", fmt, vl);
	va_end(vl);
}

void LogManager::Err(const char* fmt, ...)
{
	va_list vl;
	va_start(vl, fmt);
	DoLog(LOG_ERROR, fmt, vl);
	All("[ERROR]",fmt, vl);
	va_end(vl);
}

log_t LogManager::GetLog(int type)
{
	if (type < 0 || type > LOG_NUM)
	{
		log_t p;
		return p;
	}
	return m_logs[type];
}

void LogManager::All(const char* type, const char* fmt, va_list vl)
{
	std::string typeFmt = type;
	typeFmt += fmt;
	DoLog(LOG_ALL, typeFmt.c_str(), vl);
}

void LogManager::DoLog(int type, const char* fmt, va_list vl)
{
	log_t log = GetLog(type);
	log->log(fmt, vl);
}
