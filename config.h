#pragma once

/*
	�����ļ���Ӧ��
*/

#include "type_define.h"
#include "macro_define.h"
#include <string>

class Config
{
	SINGTONE(Config)
public:
	bool load(); // ���������ļ�
public:
	std::string scriptPath;
	std::string logPath;
};
