#pragma once

/*
	配置文件对应类
*/

#include "type_define.h"
#include "macro_define.h"
#include <string>

class Config
{
	SINGTONE(Config)
public:
	bool load(); // 加载配置文件
public:
	std::string scriptPath;
	std::string logPath;
};
