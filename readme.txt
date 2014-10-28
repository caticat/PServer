˵��
_______________________________________________________________________________________________________________________________

���й��ܣ�
���ݿ�����
�¼�ϵͳ
lua�ű�����(������ά���ȡ����)
�źŴ���
_______________________________________________________________________________________________________________________________

Ŀǰ�����⣺
����
_______________________________________________________________________________________________________________________________

�Ӻ�������⣺
boost��hppͷ�ļ���ȡ	����Ȳ�ȡ�ˣ����Ǳ��룬����boost_system�Ŀ���ܻ�����Ҫ����
_______________________________________________________________________________________________________________________________

config.lua �����ļ����ͳ���ͬ��Ŀ¼
_______________________________________________________________________________________________________________________________

�߳̽ṹ���̣�
	���߳�����
	���̴߳����̣߳�
		��Ϣ�����̣߳�ͬʱ������Ϣ���أ�
		�źŴ����̣߳�ͬʱ����ʱ��ͬ����
		��ʱ���߳�
	���̼߳�������ͨ��
	�յ���ֹ�źţ�
		�����ɫ����
		֪ͨ�����߳���ֹ
	���̵߳ȴ������߳���ֹ��ֹͣ����������
_______________________________________________________________________________________________________________________________

ͨ�����������ļ����������ݿⲢ��ͨ��ѯ

#include "db.h"

int main()
{
	if (!Config::getInstance()->load()) // ���������ļ�
		return 1;
	if (!Config::getInstance()->initDb()) // �������ݿ�
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

ͨ�����ݿ����ʹ�����ݿ��ѯ

#include "db_manager.h"
	
int main()
{
	std::string name = "";
	DbManager::getInstance()->Test(1120117, name);
	std::cout << "��ѯ�����" << name << std::endl;
}
_______________________________________________________________________________________________________________________________

���ݿ����� ��Ҫֱ��ʹ�ã�����᲻�������ġ�

#include "db.h"

int main()
{
	// ��ʼ�����ݿ����ӳ�
	DbPool* pPool = DbPool::GetInstance();
	pPool->SetConfigure("root", "", "192.168.0.183", "3306", "kuafu", "gbk");

	// ��ȡ���ݿ�����
	DbConnection dbConn;
	DB* pDb = dbConn.GetDbConnection();
	if (pDb == NULL)
	{
		std::cout << "�������ݿ����" << std::endl;
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

�¼�ϵͳʾ�����Դ���
(ͨ��ע���¼��ķ�ʽ������ʵ��ĳ�������timer)

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

lua�����ݵĶ�ȡ��֧��ֱ������ά���ȡ��

int main()
{
	PLua* pLua = new PLua("test.lua");
	std::string val = "";
	pLua->GetVal("testnum",val); // ֱ�ӻ�ȡȫ�ֱ���
	std::cout << "testnum:" << val << std::endl;
	pLua->GetVal(val, 1, "testnum"); // ��ȡ����ͨ�÷���
	std::cout << "testnum:" << val << std::endl;
	pLua->GetVal(val, 2, "t", "t1"); // ��ȡһά������
	std::cout << "t1:" << val << std::endl;
	pLua->GetVal(val, 3, "t", "t2", "t3"); // ��ȡ��ά������
	std::cout << "t3:" << val << std::endl;
	pLua->GetVal(val, 4, "t", "t2", "t4", "t5"); // ��ȡ��ά������
	std::cout << "t5:" << val << std::endl;
}

lua���룺

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

