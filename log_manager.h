#pragma once

/*
	日志管理
*/

#include "type_define.h"
#include "macro_define.h"
#include "const_define.h"
#include "plog.h"

class LogManager
{
	SINGTONE_GETINSTANCE(LogManager)

private:
	LogManager();

public:
	void Log(const char* fmt, ...);
	void Warn(const char* fmt, ...);
	void Err(const char* fmt, ...);

private:
	log_t GetLog(int type);
	void DoLog(int type, const char* fmt, va_list vl);

private:
	log_t m_logs[LOG_NUM];
};
