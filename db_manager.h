#pragma once

/*
	���ݿ����ӹ���
*/

#include "macro_define.h"

#include <string>

class DbManager
{
	SINGTONE_GETINSTANCE(DbManager);

private:
	DbManager();

public:
	/*
		����ĺ��������Լ�д�ģ�����ôд����ôд����������sqlmgrͳһ�������ݿ�����
	*/
	void Test(int id, std::string& name); // �������
};
