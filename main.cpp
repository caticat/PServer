#include "type_define.h"
#include "config.h"

#include <iostream>

#include "plua.h"

int main()
{
	if (!Config::getInstance()->load()) // 加载配置文件
	{
		std::cout << "出错了！" << std::endl;
		return 0;
	}

	PLua* pLua = new PLua("test.lua");
	std::string val = "";
	pLua->GetVal(val, 1, "testnum");
	std::cout << "testnum:" << val << std::endl;
	pLua->GetVal(val, 2, "t", "t1");
	std::cout << "t1:" << val << std::endl;
	pLua->GetVal(val, 3, "t", "t2", "t3");
	std::cout << "t3:" << val << std::endl;
	pLua->GetVal(val, 4, "t", "t2", "t4", "t5");
	std::cout << "t5:" << val << std::endl;

	return 0;
}