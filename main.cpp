#include "type_define.h"
#include "config.h"

#include <iostream>

#include "psignal.h"

bool g_isRunning = true;
int main()
{
	if (!Config::getInstance()->load()) // ���������ļ�
	{
		std::cout << "�����ˣ�" << std::endl;
		return 0;
	}

	PSignal::getInstance()->Regist();
	int idx = 0;
	while (g_isRunning)
	{
		std::cout << "�������" << idx++ << std::endl;
		sleep(2);
	}

	return 0;
}