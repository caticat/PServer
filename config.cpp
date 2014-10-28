#include "config.h"
#include "plua.h"
#include "db.h"

bool Config::load()
{
	boost::shared_ptr<PLua> pt(new PLua("config.lua"));

	// 区号
	std::string quStr = "";
	pt->GetVal("qu", quStr);
	qu = atoi(quStr.c_str());

	// 数据库
	pt->GetVal(db.user, 3, "db", quStr.c_str(), "user");
	pt->GetVal(db.pass, 3, "db", quStr.c_str(), "pass");
	pt->GetVal(db.host, 3, "db", quStr.c_str(), "host");
	pt->GetVal(db.port, 3, "db", quStr.c_str(), "port");
	pt->GetVal(db.database, 3, "db", quStr.c_str(), "database");
	pt->GetVal(db.charset, 3, "db", quStr.c_str(), "charset");

	// 客户端需求版本
	std::string clientVersionStr = "";
	pt->GetVal("clientVersion", clientVersionStr);
	clientVersion = atoi(clientVersionStr.c_str());

	// 配置路径
	pt->GetVal("scriptPath", scriptPath);
	pt->GetVal("logPath", logPath);

	return true;
}

bool Config::initDb()
{
	// 初始化数据库连接池
	DbPool* pPool = DbPool::GetInstance();
	pPool->SetConfigure(db.user, db.pass, db.host, db.port, db.database, db.charset);

	// 获取数据库连接
	DbConnection dbConn;
	DB* pDb = dbConn.GetDbConnection();
	if (pDb == NULL)
		return false;
	return true;
}
