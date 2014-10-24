˵��
_______________________________________________________________________________________________________________________________
_______________________________________________________________________________________________________________________________

Ŀǰ�����⣺
����
�߳�
���ݿ�����
_______________________________________________________________________________________________________________________________

config.lua �����ļ����ͳ���ͬ��Ŀ¼
_______________________________________________________________________________________________________________________________

�¼�ϵͳʾ�����Դ���

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

ʱ�书��ʾ����

while (true)
{
	TimeManager::getInstance()->Sync();
	std::cout << "��ǰʱ�䣺" << TimeManager::getInstance()->GetTime() << std::endl;
	sleep(1);
}
_______________________________________________________________________________________________________________________________

�źŴ���ʾ����

#include "psignal.h"
bool g_isRunning = true;
int main()
{
	PSignal::getInstance()->Regist();
	int idx = 0;
	while (g_isRunning)
	{
		std::cout << "�������" << idx++ << std::endl;
		sleep(2);
	}
	return 0;
}
_______________________________________________________________________________________________________________________________

sql����ȡ
std::string sql = SqlManager::getInstance()->Sql(SqlManager::SQL_TEST)%1;
_______________________________________________________________________________________________________________________________

log����
LogManager::getInstance()->Log("abc%s", "111"); // ��ͨ��־
LogManager::getInstance()->Warn("abc%s", "111"); // ����
LogManager::getInstance()->Error("abc%s", "111"); // ����
_______________________________________________________________________________________________________________________________

lua c++������ù��ܣ�

c++:
main�����У�
#include "plua.h"
#include "luatest.h"

PLua* p = new PLua("test.lua");
p->Call("test1", ""); // ���Ժ���1����
CScriptTest test;
test.m_id = 1;
test.m_name = "test";
std::cout << "c++���1��" << test.m_id << std::endl;
p->Call("test2", "t", &test);
std::cout << "c++���2��" << test.m_id << std::endl;

�����ļ���
script_test.h ��swig_wrap.i�а������ļ�����ʾ���п��Ÿ�lua�ĳ���ӿڣ������ڲ���ʹ��
luatest.h ������ͷ�ļ��������ڲ�ʹ��
luatest.cpp ������ʵ��

lua:
script/test.lua�У�

testnum = 10

print("lua �������")

function test1()
	print("lua test1������")
end

function test2(t)
	t:ScriptTest()
	t:Change()
	t.m_id = t.m_id + 100;
	print("lua��c���򷵻ؽ����",t:ScriptTest())
	print("lua cpp����test�������")
end

_______________________________________________________________________________________________________________________________

