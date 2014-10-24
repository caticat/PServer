#pragma once

#include "macro_define.h"

class PSignal
{
	SINGTONE(PSignal)
public:
	void Regist(); // 注册信号函数
private:
	static void SignalHandler(int signal); // 信号相应
	static void SignalHandlerWithCore(int signal); // 信号相应 产生core文件
};
