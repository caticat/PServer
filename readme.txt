说明
_______________________________________________________________________________________________________________________________
_______________________________________________________________________________________________________________________________

目前的问题：
网络
线程
数据库连接
_______________________________________________________________________________________________________________________________

config.lua 配置文件，和程序同级目录
_______________________________________________________________________________________________________________________________

事件系统示例测试代码

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

