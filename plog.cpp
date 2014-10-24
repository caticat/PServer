#include "plog.h"

#include <iostream>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

using std::cout;
using std::endl;
using std::ios;
using std::string;

CPLog::CPLog() : m_pFos(NULL),
m_strPrifix(""),
m_bShowTime(false),
m_bShowConsole(false)
{
	memset(m_cstrTime, 0, sizeof(m_cstrTime));
}

CPLog::~CPLog()
{
	log("日志文件结束");
	if (m_pFos != NULL)
	{
		m_pFos->close();
		m_pFos = NULL;
	}
}

// 初始化日志配置
// @param const string& path 日志路径
// @param const string& prifix 输出前缀
// @param ,const char* fileType 扩展名，默认为空
// @param bool showTime = false 是否输出当前时间，默认不输出
// @param bool showConsole = false 是否有当前默认控制台输出，默认不输出
// @return 是否初始化成功
bool CPLog::init(const string& path, const string& prifix, const char* fileType, bool showTime, bool showConsole)
{
	m_pFos = new std::fstream;
	if (m_pFos == NULL)
	{
		cout << "初始化日志文件流失败。\n";
		return false;
	}

	string command = "cd " + path; // 创建路径文件夹
	if (access(command.c_str(),F_OK) != 0) // 文件夹不存在
	{
		command = "mkdir -p " + path; // 创建路径文件夹
		system(command.c_str());
	}

	string fullPath = path;
	fullPath += prifix;
	fullPath += "_";
	refreshTime(true);
	fullPath += m_cstrTime;
	if (fileType != NULL)
		fullPath += fileType;

	m_pFos->open(fullPath.c_str(), ios::app|ios::out); // 创建、追加更新
	if (!m_pFos)
	{
		cout << "创建/打开日志文件失败。\n";
		return false;
	}

	m_strPrifix = prifix;
	m_bShowTime = showTime;
	m_bShowConsole = showConsole;

	log("日志文件创建");
	return true;
}

// 日志输出
void CPLog::log(const char* format, ...)
{
	if (m_pFos == NULL)
	{
		cout << "无法记录日志，日志文件不存在。\n";
		return;
	}
	if (format == NULL)
	{
		cout << "记录日志格式为空。\n";
		return;
	}

	memset(m_cstrLog, 0, sizeof(m_cstrLog));

	va_list vl;
	va_start(vl, format);

	if (m_bShowTime)
	{
		refreshTime();
		strncpy(m_cstrLog, m_cstrTime, sizeof(m_cstrLog));
		vsnprintf(m_cstrLog + strlen(m_cstrTime), sizeof(m_cstrLog)-strlen(m_cstrTime), format, vl);
	}
	else
		vsnprintf(m_cstrLog, sizeof(m_cstrLog), format, vl);

	va_end(vl);

	m_pFos->write(m_cstrLog, strlen(m_cstrLog));
	*m_pFos << '\n';
	m_pFos->flush();

	if (m_bShowConsole)
		cout << "[" << m_strPrifix.c_str() << "]" << m_cstrLog << endl;

}

void CPLog::log(const char* format, va_list vl)
{
	if (m_pFos == NULL)
	{
		cout << "无法记录日志，日志文件不存在。\n";
		return;
	}
	if (format == NULL)
	{
		cout << "记录日志格式为空。\n";
		return;
	}

	memset(m_cstrLog, 0, sizeof(m_cstrLog));

	if (m_bShowTime)
	{
		refreshTime();
		strncpy(m_cstrLog, m_cstrTime, sizeof(m_cstrLog));
		vsnprintf(m_cstrLog + strlen(m_cstrTime), sizeof(m_cstrLog)-strlen(m_cstrTime), format, vl);
	}
	else
		vsnprintf(m_cstrLog, sizeof(m_cstrLog), format, vl);

	m_pFos->write(m_cstrLog, strlen(m_cstrLog));
	*m_pFos << '\n';
	m_pFos->flush();

	if (m_bShowConsole)
		cout << "[" << m_strPrifix.c_str() << "]" << m_cstrLog << endl;
}

// 更新时间字符串
// @param bool isSimple 是否是简单时间，默认不是
void CPLog::refreshTime(bool isSimple)
{
	memset(m_cstrTime, 0, sizeof(m_cstrTime));
	time_t curTime(time(NULL));
	tm* ptm = localtime(&curTime);
	if (isSimple)
		snprintf(m_cstrTime, sizeof(m_cstrTime), "%d-%02d-%02d_%02d_%02d_%02d", (ptm->tm_year + 1900), (ptm->tm_mon + 1), (ptm->tm_mday), ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
	else
		snprintf(m_cstrTime, sizeof(m_cstrTime), "[%d-%02d-%02d_%02d:%02d:%02d]", (ptm->tm_year + 1900), (ptm->tm_mon + 1), (ptm->tm_mday), ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
}