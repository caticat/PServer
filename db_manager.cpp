#include "db_manager.h"

#include "log_manager.h"
#include "db.h"
#include "sql_manager.h"

DbManager::DbManager()
{
	DbPool* pPool = DbPool::GetInstance();
	pPool->SetConfigure("root", "", "192.168.0.183", "3306", "kuafu", "gbk");
}

void DbManager::Test(int id, std::string& name)
{
	GETDB;
	if (!pDb->Query((SqlManager::getInstance()->Sql(SqlManager::SQL_TEST) % id).str().c_str()))
		return;
	char** row = pDb->GetRow();
	if (row == NULL)
		return;
	name = row[1];
}
