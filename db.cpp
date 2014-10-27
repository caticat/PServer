#include "db.h"

#include "macro_define.h"
#include "const_define.h"
#include "log_manager.h"

#include <mysql.h>

DB::DB()
{
	m_pMysql = new MYSQL;
	m_pResult = NULL;
	mysql_init(m_pMysql);
}

DB::~DB()
{
	ClearResult();
	mysql_close(m_pMysql);
	RELEASE(m_pMysql);
}

bool DB::Connect(const char* user, const char* pass, const char* host, const char* port, const char* database, const char* charset)
{
	mysql_options(m_pMysql, MYSQL_OPT_CONNECT_TIMEOUT, (char*)&DB_CONN_TIMEOUT);
	mysql_options(m_pMysql, MYSQL_OPT_READ_TIMEOUT, (char*)&DB_RECV_TIMEOUT);
	my_bool reconnect = 1;
	mysql_options(m_pMysql, MYSQL_OPT_RECONNECT, &reconnect);

	int iPort = atoi(port);
	if (!mysql_real_connect(m_pMysql, host, user, pass, database, iPort, NULL, CLIENT_MULTI_STATEMENTS | CLIENT_MULTI_RESULTS))
		return false;
	if (mysql_select_db(m_pMysql, database))
		return false;
	if (mysql_set_character_set(m_pMysql, charset))
		return false;
	return true;
}

bool DB::Query(const char* sql)
{
	ClearResult();
	if (mysql_query(m_pMysql,sql) == 0)
	{
		m_pResult = mysql_store_result(m_pMysql);
		return true;
	}
	else
	{
		LogManager::getInstance()->Err("%s", sql);
		LogManager::getInstance()->Err("sql error:%s", GetError());
		return false;
	}
}

char** DB::GetRow()
{
	if (m_pResult == NULL)
		return NULL;
	return mysql_fetch_row(m_pResult);
}

int DB::GetRowNum()
{
	return (int)mysql_num_rows(m_pResult);
}

uint64 DB::GetInsertId()
{
	return (uint64)mysql_insert_id(m_pMysql);
}

const char* DB::GetError()
{
	return mysql_error(m_pMysql);
}

void DB::ClearResult()
{
	if (m_pResult == NULL)
		return;
	do
	{
		mysql_free_result(m_pResult);
	} while (mysql_next_result(m_pMysql) > 0);
	m_pResult = NULL;
}

DbConnection::~DbConnection()
{
	if (m_pDb != NULL && DbPool::m_pData != NULL)
		DbPool::m_pData->PushConnection(m_pDb);
	//std::cout << "数据库连接析构函数调用" << std::endl;
}

DB* DbConnection::GetDbConnection()
{
	if (DbPool::m_pData == NULL)
		return NULL;
	if (m_pDb == NULL)
	{
		DbPoolData* pData = DbPool::m_pData;
		DB* pDb = pData->PopConnection();
		if (pDb == NULL)
		{
			pData->m_pPool->AddConnection();
			pDb = pData->PopConnection();
		}
		m_pDb = pDb;
	}
	return m_pDb;
}

DB* DbPoolData::PopConnection()
{
	boost::mutex::scoped_lock _lock(m_mutex);
	if (m_sDb.size() <= 0)
		return NULL;
	DB* pDb = m_sDb.top();
	m_sDb.pop();
	return pDb;
}

void DbPoolData::PushConnection(DB* pDb)
{
	boost::mutex::scoped_lock _lock(m_mutex);
	m_sDb.push(pDb);
}

DbPoolData* DbPool::m_pData = NULL;
DbPool::DbPool()
{
	if (m_pData == NULL)
	{
		m_pData = new DbPoolData;
		m_pData->m_pPool = this;
	}
}

DbPool::~DbPool()
{
	RELEASE(m_pData);
}

void DbPool::SetConfigure(std::string user, std::string pass, std::string host, std::string port, std::string database, std::string charset)
{
	if (m_pData == NULL)
		return;
	m_pData->m_user = user;
	m_pData->m_pass = pass;
	m_pData->m_host = host;
	m_pData->m_port = port.c_str();
	m_pData->m_database = database;
	m_pData->m_charset = charset;
}

bool DbPool::AddConnection()
{
	if (m_pData == NULL)
		return false;
	DB* pDb = new DB;
	if (!pDb->Connect(m_pData->m_user.c_str(),m_pData->m_pass.c_str(),m_pData->m_host.c_str(),m_pData->m_port.c_str(),m_pData->m_database.c_str(),m_pData->m_charset.c_str()))
	{
		LogManager::getInstance()->Err("DbPool::AddConnection:%s", pDb->GetError());
		RELEASE(pDb);
		return false;
	}
	m_pData->PushConnection(pDb);
	return true;
}

DbPool* DbPool::GetInstance()
{
	if (m_pData == NULL)
	{
		return new DbPool;
	}
	else
	{
		return m_pData->m_pPool;
	}
}
