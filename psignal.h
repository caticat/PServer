#pragma once

#include "macro_define.h"

class PSignal
{
	SINGTONE(PSignal)
public:
	void Regist(); // ע���źź���
private:
	static void SignalHandler(int signal); // �ź���Ӧ
	static void SignalHandlerWithCore(int signal); // �ź���Ӧ ����core�ļ�
};
