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
	log("��־�ļ�����");
	if (m_pFos != NULL)
	{
		m_pFos->close();
		m_pFos = NULL;
	}
}

// ��ʼ����־����
// @param const string& path ��־·��
// @param const string& prifix ���ǰ׺
// @param ,const char* fileType ��չ����Ĭ��Ϊ��
// @param bool showTime = false �Ƿ������ǰʱ�䣬Ĭ�ϲ����
// @param bool showConsole = false �Ƿ��е�ǰĬ�Ͽ���̨�����Ĭ�ϲ����
// @return �Ƿ��ʼ���ɹ�
bool CPLog::init(const string& path, const string& prifix, const char* fileType, bool showTime, bool showConsole)
{
	m_pFos = new std::fstream;
	if (m_pFos == NULL)
	{
		cout << "��ʼ����־�ļ���ʧ�ܡ�\n";
		return false;
	}

	string command = "cd " + path; // ����·���ļ���
	if (access(command.c_str(),F_OK) != 0) // �ļ��в�����
	{
		command = "mkdir -p " + path; // ����·���ļ���
		system(command.c_str());
	}

	string fullPath = path;
	fullPath += prifix;
	fullPath += "_";
	refreshTime(true);
	fullPath += m_cstrTime;
	if (fileType != NULL)
		fullPath += fileType;

	m_pFos->open(fullPath.c_str(), ios::app|ios::out); // ������׷�Ӹ���
	if (!m_pFos)
	{
		cout << "����/����־�ļ�ʧ�ܡ�\n";
		return false;
	}

	m_strPrifix = prifix;
	m_bShowTime = showTime;
	m_bShowConsole = showConsole;

	log("��־�ļ�����");
	return true;
}

// ��־���
void CPLog::log(const char* format, ...)
{
	if (m_pFos == NULL)
	{
		cout << "�޷���¼��־����־�ļ������ڡ�\n";
		return;
	}
	if (format == NULL)
	{
		cout << "��¼��־��ʽΪ�ա�\n";
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
		cout << "�޷���¼��־����־�ļ������ڡ�\n";
		return;
	}
	if (format == NULL)
	{
		cout << "��¼��־��ʽΪ�ա�\n";
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

// ����ʱ���ַ���
// @param bool isSimple �Ƿ��Ǽ�ʱ�䣬Ĭ�ϲ���
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