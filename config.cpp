#include "config.h"
#include "plua.h"
#include "db.h"

bool Config::load()
{
	boost::shared_ptr<PLua> pt(new PLua("config.lua"));

	// ����
	std::string quStr = "";
	pt->GetVal("qu", quStr);
	qu = atoi(quStr.c_str());

	// ���ݿ�
	pt->GetVal(db.user, 3, "db", quStr.c_str(), "user");
	pt->GetVal(db.pass, 3, "db", quStr.c_str(), "pass");
	pt->GetVal(db.host, 3, "db", quStr.c_str(), "host");
	pt->GetVal(db.port, 3, "db", quStr.c_str(), "port");
	pt->GetVal(db.database, 3, "db", quStr.c_str(), "database");
	pt->GetVal(db.charset, 3, "db", quStr.c_str(), "charset");

	// �ͻ�������汾
	std::string clientVersionStr = "";
	pt->GetVal("clientVersion", clientVersionStr);
	clientVersion = atoi(clientVersionStr.c_str());

	// ����·��
	pt->GetVal("scriptPath", scriptPath);
	pt->GetVal("logPath", logPath);

	return true;
}

bool Config::initDb()
{
	// ��ʼ�����ݿ����ӳ�
	DbPool* pPool = DbPool::GetInstance();
	pPool->SetConfigure(db.user, db.pass, db.host, db.port, db.database, db.charset);

	// ��ȡ���ݿ�����
	DbConnection dbConn;
	DB* pDb = dbConn.GetDbConnection();
	if (pDb == NULL)
		return false;
	return true;
}
