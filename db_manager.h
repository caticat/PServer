#pragma once

/*
	数据库连接管理
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
		下面的函数都是自己写的，该怎么写就怎么写，这个类配合sqlmgr统一处理数据库内容
	*/
	void Test(int id, std::string& name); // 测试语句
};
