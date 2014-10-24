#pragma once

/*
	时间管理
	定时器每秒调用更新
	外部调用获取时间信息

	注：
	需要在所有使用
*/

#include "macro_define.h"
#include <time.h>

class TimeManager
{
	SINGTONE_GETINSTANCE(TimeManager)
private:
	TimeManager() : m_curTime(0) {}
public:
	void Sync() { m_curTime = time(NULL); }; // 同步时间
	time_t GetTime() { return m_curTime; } // 获取当前时间

private:
	time_t m_curTime; // 当前时间
};

/* 相关代码记录
#include <stdio.h>
#include <time.h>

void main()
{
	time_t sec;       //typedef long time_t
	struct tm * curTime;

	sec = time(NULL);           //获取时间，从1970.1.1到现在的秒数,也可以写成 time(&sec);
	curTime = localtime(&sec);  //把sec转换为当地时间，存于时间结构体curTime中，注意返回值是struct tm* 类型的

	printf("asctime(curTime): %s\n", asctime(curTime));
	//asctime(struct tm*)函数将curTime转化成标准ASCII时间格式

	printf("ctime(curTime):   %s\n", ctime(&sec));
	//ctime(time_t*)直接将sec转换成标准时间格式

	printf("%d-%d-%d %d:%d:%d\n", 1900 + curTime->tm_year, 1 + curTime->tm_mon,
		curTime->tm_mday, curTime->tm_hour, curTime->tm_min, curTime->tm_sec);
	//手动输出struct tm中的字段，其中月份加1，年份要加1900，
	//sec记录的是1970到现在的秒数，为什么是加1900呢？？？

	printf("\nsec = %ld\nmktime(curTime) = %ld\n", sec, mktime(curTime));
	//time_t mktime(struct tm*)将curTime反向转化成1970到现在的秒数
}
*/
