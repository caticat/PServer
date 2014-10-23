#pragma once

#include "type_define.h"
#include "macro_define.h"
#include <string>

class Config
{
	SINGTONE(Config)
public:
	bool load(); // ╪стьеДжцнд╪Ч
public:
	std::string scriptPath;
	std::string logPath;
};
