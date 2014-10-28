#pragma once

/*
	脚本类
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

	void Call(const char* func, const char* fmt, ...); // 函数调用
	void GetVal(const char* key, std::string& value); // 获取一个值
	void GetVal(std::string& value, int argn, ...); // 获取多层的数据 需要参数个数

	void Reload(); // 重新加载

private:
	void Call(const char* func, const char* fmt, va_list vl); // 函数调用(自定义参数设置在这里做)
	bool PushPointer(void* p, const char* type, int owned = 0); // 入栈一个指针参数
	void GetValList(std::string& value, int argn, va_list vl, bool isFirstIn = true); // 获取多层的数据 需要参数个数
	void StackDump(); // lua堆栈输出
	
private:
	lua_State* m_pLua;
	char m_file[PATH_LEN];
	boost::recursive_mutex m_mutex;
};
