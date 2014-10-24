#pragma once

/*
	测试脚本功能用cpp类
*/

#include <iostream>
#include <string>

//void test()
//{
//	std::cout << "cpp test 输出测试" << std::endl;
//}

class CScriptTest
{
public:
	//CScriptTest();
	int m_id;
	std::string m_name;
	int ScriptTest();
	void Change();
};
