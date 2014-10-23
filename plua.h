#pragma once

/*
	�ű���
*/

#include <lua.hpp>
#include "const_define.h"
#include <string>

class PLua
{
public:
	PLua(const char* file);
	~PLua();

	void Call(const char* func, const char* fmt, ...); // ��������
	void GetVal(const char* key, std::string& value); // ��ȡһ��ֵ

	void Reload(); // ���¼���

private:
	void Call(const char* func, const char* fmt, va_list vl); // ��������(�Զ������������������)
	bool PushPointer(void* p, const char* type, int owned = 0); // ��ջһ��ָ�����
	
private:
	lua_State* m_pLua;
	char m_file[PATH_LEN];
};
