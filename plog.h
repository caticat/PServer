#pragma once

/*
��־�����

#include "plog.h"

// ���Է���
int main()
{
CPLog log;
log.init("E:/test/","Error",".txt",true,true);
log.log("111");
log.log("222");
log.log("333");
log.log("%s:%d","lala",256);
return 0;
}
*/

#include <string>
#include <fstream>

class CPLog
{
public:
	CPLog();
	~CPLog();

public:
	bool init(const std::string& path, const std::string& prifix, const char* fileType = NULL, bool showTime = false, bool showConsole = false); // ��־·�������ǰ׺���Ƿ������ǰʱ�䣬�Ƿ��е�ǰĬ�Ͽ���̨���
	void log(const char* format, ...); // ��־���
	void log(const char* format, va_list vl); // ��־���

private:
	void refreshTime(bool isSimple = false); // ����ʱ���ַ���

private:
	std::fstream* m_pFos; // �ļ���ָ��
	std::string m_strPrifix; // ǰ׺
	bool m_bShowTime; // �Ƿ���ʾʱ��
	bool m_bShowConsole; // �Ƿ����������̨��Ĭ�������ʽ��
	char m_cstrTime[32]; // ��ʱʱ���ַ���
	char m_cstrLog[256]; // ��־�������
};