#include "type_define.h"
#include "config.h"

#include <iostream>


int main()
{
	if (!Config::getInstance()->load()) // ���������ļ�
	{
		std::cout << "�����ˣ�" << std::endl;
		return 0;
	}

	return 0;
}