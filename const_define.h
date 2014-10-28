#pragma once

/*
	常量定义
*/

enum CONST_DEF
{
	PATH_LEN = 128, // 路径长度
	TIMER_TIMEOUT = 10, // 定时器调用间隔
	SLEEP_TIMEOUT = 1, // 睡眠时间间隔
};

enum THREAD_DEF
{
	THREAD_PACKAGEDEAL = 0, // 消息处理线程（同时负责消息返回）
	THREAD_TIMER = 1, // 信号处理线程（同时负责时间同步）
	THREAD_SIGNAL = 2, // 定时器线程
	THREAD_NUM = 3, // 主线程以外的线程数量
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
