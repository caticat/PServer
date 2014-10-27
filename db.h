#pragma once

/*
	���ݿ����Ӷ���
	���ݿ�����
	���ݿ����ӳ�����
	���ݿ����ӳ�
*/

#include "type_define.h"

#include <string>
#include <stack>
#include <boost/thread/mutex.hpp>

struct st_mysql;
struct st_mysql_res;
class DbPool;

// ���ݿ����Ӷ���
class DB
{
public:
	DB();
	~DB();
	bool Connect(const char* user, const char* pass, const char* host, const char* port, const char* database, const char* charset);

public:
	bool Query(const char* sql); // ����sql
	char** GetRow(); // ��ȡ�����
	int GetRowNum(); // ��ȡ�������
	uint64 GetInsertId(); // ��ȡ�ϴβ���id
	const char* GetError(); // ��ȡ�ϴδ�����Ϣ

private:
	void ClearResult(); // ��ա��ͷŽ����
	
private:
	st_mysql* m_pMysql;
	st_mysql_res* m_pResult;
};

// ���ݿ�����
class DbConnection
{
public:
	DbConnection() : m_pDb(NULL) {}
	~DbConnection();

public:
	DB* GetDbConnection();

private:
	void* operator new(size_t t){ return malloc(t); }

private:
	DB* m_pDb;
};

// ���ݿ����ӳ�����
class DbPoolData
{
public:
	DB* PopConnection();
	void PushConnection(DB* pDb);

public:
	DbPool* m_pPool;
	std::stack<DB*> m_sDb;
	std::string m_user;
	std::string m_pass;
	std::string m_host;
	std::string m_port;
	std::string m_database;
	std::string m_charset;

private:
	boost::mutex m_mutex;
};

// ���ݿ����ӳ�
class DbPool
{
private:
	DbPool();
	~DbPool();
public:
	void SetConfigure(std::string user, std::string pass, std::string host, std::string port, std::string database, std::string charset); // �����ļ�
	bool AddConnection(); // ��������
	static DbPool* GetInstance(); // ��ȡʵ��

private:
	static DbPoolData* m_pData;
	friend class DbConnection;
};
