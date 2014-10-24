#pragma once

/*
	sql������
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
		SQL_MAX, // sql���鳤��
	};

public:
	boost::format& Sql(SQLS idx); // ��ȡsql

private:
	boost::format m_sqls[SQL_MAX];
};
