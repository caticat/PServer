说明
_______________________________________________________________________________________________________________________________

现有功能：
数据库连接
事件系统
lua脚本调用(包含多维表读取功能)
信号处理
_______________________________________________________________________________________________________________________________

目前的问题：
网络
_______________________________________________________________________________________________________________________________

延后处理的问题：
boost的hpp头文件提取	这个先不取了，不是必须，而且boost_system的库可能还是需要编译
_______________________________________________________________________________________________________________________________

config.lua 配置文件，和程序同级目录
_______________________________________________________________________________________________________________________________

线程结构流程：
	主线程启动
	主线程创建线程：
		消息处理线程（同时负责消息返回）
		信号处理线程（同时负责时间同步）
		定时器线程
	主线程监听网络通信
	收到终止信号：
		保存角色数据
		通知所有线程终止
	主线程等待所有线程终止后，停止服务器功能
_______________________________________________________________________________________________________________________________

通过加载配置文件，连接数据库并普通查询

#include "db.h"

int main()
{
	if (!Config::getInstance()->load()) // 加载配置文件
		return 1;
	if (!Config::getInstance()->initDb()) // 连接数据库
		return 2;

	DbConnection dbConn;
	DB* pDb = dbConn.GetDbConnection();
	if (pDb == NULL)
		return 3;
	if (!pDb->Query("select name from role_info where id = 1120117;"))
		return 4;
	char** row = pDb->GetRow();
	if (row == NULL || row[0] == NULL)
		return 5;
	std::cout << "name is :" << row[0] << std::endl;
}
_______________________________________________________________________________________________________________________________

通过数据库管理使用数据库查询

#include "db_manager.h"
	
int main()
{
	std::string name = "";
	DbManager::getInstance()->Test(1120117, name);
	std::cout << "查询结果：" << name << std::endl;
}
_______________________________________________________________________________________________________________________________

数据库连接 不要直接使用，后面会不方便管理的。

#include "db.h"

int main()
{
	// 初始化数据库连接池
	DbPool* pPool = DbPool::GetInstance();
	pPool->SetConfigure("root", "", "192.168.0.183", "3306", "kuafu", "gbk");

	// 获取数据库连接
	DbConnection dbConn;
	DB* pDb = dbConn.GetDbConnection();
	if (pDb == NULL)
	{
		std::cout << "连接数据库出错" << std::endl;
		return 1;
	}
	if (!pDb->Query("select id,name,level from role_info order by id limit 3;"))
		return -2;
	char** row = NULL;
	while ((row = pDb->GetRow()) != NULL)
	{
		int64 id = atoi(row[0]);
		std::string name = row[1];
		int level = atoi(row[2]);
		std::cout << "id:" << id << ",name:" << name << ",level:" << level << std::endl;
	}
}
_______________________________________________________________________________________________________________________________

事件系统示例测试代码
(通过注册事件的方式，可以实现某个子类的timer)

#include "event_manager.h"
#include "pevent.h"
#include <boost/bind.hpp>
using namespace std;
void Eve1(const PEvent& eve)
{
	cout << "1" << endl;
}

void Eve2(const PEvent& eve)
{
	cout << "2" << endl;
}

void Eve3(const PEvent& eve)
{
	PEvent2 eve2 = (const PEvent2&)eve;
	cout << "3:" << eve2.data << endl;
}

class EveTest
{
public:
	void Eve4(const PEvent& eve)
	{
		PEvent2 eve2 = (const PEvent2&)eve;
		cout << "4:" << eve2.data << endl;
	}
};

int main()
{
	EventManager* pEveMgr = EventManager::getInstance();
	pEveMgr->Push(Eve_Test, boost::bind(&Eve1,_1));
	pEveMgr->Push(Eve_Test1, boost::bind(&Eve2, _1));
	pEveMgr->Push(Eve_Test1, boost::bind(&Eve3, _1));

	EveTest eveTest;
	pEveMgr->Push(Eve_Test1, boost::bind(&EveTest::Eve4,&eveTest,_1));

	PEvent eve(Eve_Test);
	pEveMgr->Launch(eve);
	PEvent2 eve2(Eve_Test1);
	eve2.data = 100;
	pEveMgr->Launch(eve2);
	pEveMgr->Pop(Eve_Test1, boost::bind(&EveTest::Eve4, &eveTest, _1));
	pEveMgr->Launch(eve2);
	return 0;
}

_______________________________________________________________________________________________________________________________

时间功能示例：

while (true)
{
	TimeManager::getInstance()->Sync();
	std::cout << "当前时间：" << TimeManager::getInstance()->GetTime() << std::endl;
	sleep(1);
}
_______________________________________________________________________________________________________________________________

信号处理示例：

#include "psignal.h"
bool g_isRunning = true;
int main()
{
	PSignal::getInstance()->Regist();
	int idx = 0;
	while (g_isRunning)
	{
		std::cout << "输出测试" << idx++ << std::endl;
		sleep(2);
	}
	return 0;
}
_______________________________________________________________________________________________________________________________

sql语句获取
std::string sql = SqlManager::getInstance()->Sql(SqlManager::SQL_TEST)%1;
_______________________________________________________________________________________________________________________________

log调用
LogManager::getInstance()->Log("abc%s", "111"); // 普通日志
LogManager::getInstance()->Warn("abc%s", "111"); // 警告
LogManager::getInstance()->Error("abc%s", "111"); // 错误
_______________________________________________________________________________________________________________________________

lua中数据的读取（支持直读，多维表读取）

int main()
{
	PLua* pLua = new PLua("test.lua");
	std::string val = "";
	pLua->GetVal("testnum",val); // 直接获取全局变量
	std::cout << "testnum:" << val << std::endl;
	pLua->GetVal(val, 1, "testnum"); // 获取变量通用方法
	std::cout << "testnum:" << val << std::endl;
	pLua->GetVal(val, 2, "t", "t1"); // 获取一维表数据
	std::cout << "t1:" << val << std::endl;
	pLua->GetVal(val, 3, "t", "t2", "t3"); // 获取二维表数据
	std::cout << "t3:" << val << std::endl;
	pLua->GetVal(val, 4, "t", "t2", "t4", "t5"); // 获取三维表数据
	std::cout << "t5:" << val << std::endl;
}

lua代码：

testnum = 10
t = {
	t1 = 20,
	t2 = {
		t3 = 30,
		t4 = {
			t5 = 40,
		},
	},
}
_______________________________________________________________________________________________________________________________

lua c++互相调用功能：

c++:
main函数中：
#include "plua.h"
#include "luatest.h"

PLua* p = new PLua("test.lua");
p->Call("test1", ""); // 测试函数1调用
CScriptTest test;
test.m_id = 1;
test.m_name = "test";
std::cout << "c++输出1：" << test.m_id << std::endl;
p->Call("test2", "t", &test);
std::cout << "c++输出2：" << test.m_id << std::endl;

程序文件：
script_test.h 在swig_wrap.i中包含的文件，表示所有开放给lua的程序接口，程序内部不使用
luatest.h 正常类头文件，程序内部使用
luatest.cpp 正常类实现

lua:
script/test.lua中：

testnum = 10

print("lua 测试输出")

function test1()
	print("lua test1被调用")
end

function test2(t)
	t:ScriptTest()
	t:Change()
	t.m_id = t.m_id + 100;
	print("lua中c程序返回结果：",t:ScriptTest())
	print("lua cpp函数test调用完成")
end

_______________________________________________________________________________________________________________________________

