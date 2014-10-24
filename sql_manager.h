#pragma once

/*
	sql语句管理
*/

#include "macro_define.h"
#include <boost/format.hpp>

class SqlManager
{
	SINGTONE_GETINSTANCE(SqlManager)

private:
	SqlManager();

public:
	enum SQLS
	{
		SQL_TEST,
		SQL_MAX, // sql数组长度
	};

public:
	boost::format& Sql(SQLS idx); // 获取sql

private:
	boost::format m_sqls[SQL_MAX];
};
