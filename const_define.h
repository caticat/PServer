#pragma once

/*
	常量定义
*/

enum CONST_DEF
{
	PATH_LEN = 128, // 路径长度
};

enum LOG_DEF
{
	LOG_NORMAL = 0, // 普通日志
	LOG_WARN = 1, // 警告
	LOG_ERROR = 2, // 错误
	LOG_NUM = 3, // 日志类型数
};

// 数据库连接
extern const int DB_CONN_TIMEOUT; // 连接超时时间
extern const int DB_RECV_TIMEOUT; // 返回超时时间
