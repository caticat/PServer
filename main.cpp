#include "type_define.h"
#include "config.h"
#include "server.h"

#include "time_manager.h"
#include "log_manager.h"

PServer* g_pServer;

int main()
{
	TimeManager::getInstance()->Sync();
	if (!Config::getInstance()->load()) // 加载配置文件
		return 1;
	if (!Config::getInstance()->initDb()) // 连接数据库
		return 2;

	// 服务器创建
	PServer* pServer = new PServer;
	if (pServer->Init()) // 初始化
	{
		g_pServer = pServer;
		pServer->Run(); // 启动线程
	}

	// 删除服务器对象
	delete pServer;
	return 0;
}