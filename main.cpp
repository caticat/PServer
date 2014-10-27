#include "type_define.h"
#include "config.h"

#include <iostream>


int main()
{
	if (!Config::getInstance()->load()) // 加载配置文件
	{
		std::cout << "出错了！" << std::endl;
		return 0;
	}

	return 0;
}