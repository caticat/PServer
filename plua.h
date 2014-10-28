#pragma once

/*
	�ű���
*/

#include <lua.hpp>
#include "const_define.h"
#include <string>
#include <boost/thread/pthread/recursive_mutex.hpp>

class PLua
{
public:
	PLua(const char* file);
	~PLua();

	void Call(const char* func, const char* fmt, ...); // ��������
	void GetVal(const char* key, std::string& value); // ��ȡһ��ֵ
	void GetVal(std::string& value, int argn, ...); // ��ȡ�������� ��Ҫ��������

	void Reload(); // ���¼���

private:
	void Call(const char* func, const char* fmt, va_list vl); // ��������(�Զ������������������)
	bool PushPointer(void* p, const char* type, int owned = 0); // ��ջһ��ָ�����
	void GetValList(std::string& value, int argn, va_list vl, bool isFirstIn = true); // ��ȡ�������� ��Ҫ��������
	void StackDump(); // lua��ջ���
	
private:
	lua_State* m_pLua;
	char m_file[PATH_LEN];
	boost::recursive_mutex m_mutex;
};
