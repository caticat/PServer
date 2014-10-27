#pragma once

/*
	数据库连接对象
	数据库连接
	数据库连接池数据
	数据库连接池
*/

#include "type_define.h"

#include <string>
#include <stack>
#include <boost/thread/mutex.hpp>

struct st_mysql;
struct st_mysql_res;
class DbPool;

// 数据库连接对象
class DB
{
public:
	DB();
	~DB();
	bool Connect(const char* user, const char* pass, const char* host, const char* port, const char* database, const char* charset);

public:
	bool Query(const char* sql); // 运行sql
	char** GetRow(); // 获取结果行
	int GetRowNum(); // 获取结果行数
	uint64 GetInsertId(); // 获取上次插入id
	const char* GetError(); // 获取上次错误信息

private:
	void ClearResult(); // 清空、释放结果集
	
private:
	st_mysql* m_pMysql;
	st_mysql_res* m_pResult;
};

// 数据库连接
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

// 数据库连接池数据
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

// 数据库连接池
class DbPool
{
private:
	DbPool();
	~DbPool();
public:
	void SetConfigure(std::string user, std::string pass, std::string host, std::string port, std::string database, std::string charset); // 配置文件
	bool AddConnection(); // 增加连接
	static DbPool* GetInstance(); // 获取实例

private:
	static DbPoolData* m_pData;
	friend class DbConnection;
};
