#pragma once

/*
	脚本类
*/

#include <lua.hpp>
#include "const_define.h"
#include <string>

class PLua
{
public:
	PLua(const char* file);
	~PLua();

	void Call(const char* func, const char* fmt, ...); // 函数调用
	void GetVal(const char* key, std::string& value); // 获取一个值

	void Reload(); // 重新加载

private:
	void Call(const char* func, const char* fmt, va_list vl); // 函数调用(自定义参数设置在这里做)
	bool PushPointer(void* p, const char* type, int owned = 0); // 入栈一个指针参数
	
private:
	lua_State* m_pLua;
	char m_file[PATH_LEN];
};
