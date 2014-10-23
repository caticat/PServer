#include "plua.h"
#include "log_manager.h"
#include "config.h"
#include "macro_define.h"
#include "swig_wrap.h"
#include <string>

extern "C" int luaopen_c(lua_State*);

PLua::PLua(const char* file)
{
	snprintf(m_file, sizeof(m_file), "%s%s", Config::getInstance()->scriptPath.c_str(), file);
	m_pLua = luaL_newstate();
	luaL_openlibs(m_pLua);
	luaopen_c(m_pLua);
	Reload();
}

PLua::~PLua()
{

}

void PLua::Call(const char* func, const char* fmt, ...)
{
	// 参数处理
	va_list vl;
	va_start(vl, fmt);

	Call(func, fmt, vl);

	va_end(vl);
}

void PLua::GetVal(const char* key, std::string& value)
{
	lua_getglobal(m_pLua, key);
	value = lua_tostring(m_pLua, -1);
	lua_pop(m_pLua, 1);
}

void PLua::Reload()
{
	if ((luaL_loadfile(m_pLua, m_file) != 0) || (lua_pcall(m_pLua, 0, 0, 0) != 0))
		LogManager::getInstance()->Err("load file:%s failed, reason:%s", m_file, lua_tostring(m_pLua, -1));
}

// 函数调用(自定义参数设置在这里做)
void PLua::Call(const char* func, const char* fmt, va_list vl)
{
	lua_settop(m_pLua, 0); // 重置堆栈
	lua_getglobal(m_pLua, func); // 获取函数地址

	// 参数处理
	int narg = 0; // 参数个数
	bool isArgEnd = false; // 是否参数列表结束
	while (*fmt && (!isArgEnd))
	{
		switch (*fmt++)
		{
		case 'd': // double
		{
					  lua_pushnumber(m_pLua, va_arg(vl, int));
					  break;
		}
		case 'i': // int
		{
					  lua_pushnumber(m_pLua, va_arg(vl, int));
					  break;
		}
		case 's': // string
		{
					  lua_pushstring(m_pLua, va_arg(vl, char*));
					  break;
		}
		case 'u': // user 自定义类型
		{
					  if (!PushPointer(va_arg(vl, void*), "CUser*"))
						  LogManager::getInstance()->Err("lua call func:%s error.", func);
					  break;
		}
		DEFINE_LUA_TYPE('t', "CScriptTest*")
		case '>': // 参数返回值分隔符
		{
					  isArgEnd = true;
		}
		default:
			LogManager::getInstance()->Err("lua call func:%s error. invalid argument", func);
		}
		++narg;
	}

	// 函数调用
	int nres = strlen(fmt); // 返回值个数
	if (lua_pcall(m_pLua, narg, nres, 0) != 0) // 函数调用
	{
		LogManager::getInstance()->Err("lua call func:%s error. %s", func, lua_tostring(m_pLua, -1));
		return;
	}

	// 获取返回值
	nres = -nres; // 结果的索引
	while (*fmt)
	{
		switch (*fmt++)
		{
		case 'd': // double
		{
					  if (!lua_isnumber(m_pLua, nres))
					  {
						  LogManager::getInstance()->Err("lua call func:%s error. wrong result type.", func);
						  return;
					  }
					  *va_arg(vl, double*) = lua_tonumber(m_pLua, nres);
					  break;
		}
		case 'i': // int
		{
					  if (!lua_isnumber(m_pLua, nres))
					  {
						  LogManager::getInstance()->Err("lua call func:%s error. wrong result type.", func);
						  return;
					  }
					  *va_arg(vl, int*) = lua_tonumber(m_pLua, nres);
					  break;
		}
		case 's': // string
		{
					  if (!lua_isstring(m_pLua, nres))
					  {
						  LogManager::getInstance()->Err("lua call func:%s error. wrong result type.", func);
						  return;
					  }
					  *va_arg(vl, const char**) = lua_tostring(m_pLua, nres);
					  break;
		}
		default:
		{
				   LogManager::getInstance()->Err("lua call func:%s error. invalid option(%c).", func, *(fmt - 1));
				   break;
		}
		}
		++nres;
	}
}

bool PLua::PushPointer(void* p, const char* type, int owned)
{
	swig_type_info* pTypeInfo = SWIG_TypeQuery(m_pLua, type);
	if (pTypeInfo == NULL)
		return false;
	SWIG_NewPointerObj(m_pLua, p, pTypeInfo, owned);
	return true;
}
