#include "luatest.h"

//CScriptTest::CScriptTest() : m_id(0), m_name("test")
//{
//
//}

int CScriptTest::ScriptTest()
{
	std::cout << "cpp CScriptTest::ScriptTest Êä³ö²âÊÔ" << std::endl;
	std::cout << "id:" << m_id << std::endl;
	std::cout << "name:" << m_name << std::endl;
	return 3;
}

void CScriptTest::Change()
{
	++m_id;
	m_name = m_name + "1";
}
