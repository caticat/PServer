#pragma once

/*
	�����ļ���Ӧ��
*/

#include "type_define.h"
#include "macro_define.h"
#include <string>

struct DBConfig // ���ݿ�����
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
	bool load(); // ���������ļ�
	bool initDb(); // ��ʼ��DB
public:
	int qu; // ����
	DBConfig db; // ���ݿ�����
	int clientVersion; // �ͻ�������汾
	std::string scriptPath;
	std::string logPath;
};
