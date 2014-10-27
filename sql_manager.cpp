#include "sql_manager.h"
#include "log_manager.h"

SqlManager::SqlManager()
{
	m_sqls[SQL_TEST] = boost::format("select id,name from role_info where id = %1%;");
}

boost::format& SqlManager::Sql(SQLS idx)
{
	if (idx < 0 || idx >= SQL_MAX)
	{
		LogManager::getInstance()->Err("get sql error.idx=%d", idx);
		static boost::format sqlNull("");
		return sqlNull;
	}
	return m_sqls[idx];
}
