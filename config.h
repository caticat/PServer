#pragma once

/*
	配置文件对应类
*/

#include "type_define.h"
#include "macro_define.h"
#include <string>

struct DBConfig // 数据库配置
{
	DBConfig() : user(""), pass(""), host(""), port(""), database(""), charset(""){}
	std::string user;
	std::string pass;
	std::string host;
	std::string port;
	std::string database;
	std::string charset;
};

class Config
{
	SINGTONE(Config)
public:
	bool load(); // 加载配置文件
	bool initDb(); // 初始化DB
public:
	int qu; // 区号
	DBConfig db; // 数据库配置
	int clientVersion; // 客户端需求版本
	std::string scriptPath;
	std::string logPath;
};
