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
	// ��������
	va_list vl;
	va_start(vl, fmt);

	Call(func, fmt, vl);

	va_end(vl);
}

void PLua::GetVal(const char* key, std::string& value)
{
	lua_settop(m_pLua, 0); // ���ö�ջ
	lua_getglobal(m_pLua, key);
	if (lua_isstring(m_pLua,-1))
		value = lua_tostring(m_pLua, -1);
	lua_pop(m_pLua, 1);
}

// ��ȡ�������� ��Ҫ��������
void PLua::GetVal(std::string& value, int argn, ...)
{
	lua_settop(m_pLua, 0); // ���ö�ջ
	va_list vl;
	va_start(vl, argn);
	GetValList(value, argn, vl);
	va_end(vl);
}

void PLua::Reload()
{
	if ((luaL_loadfile(m_pLua, m_file) != 0) || (lua_pcall(m_pLua, 0, 0, 0) != 0))
		LogManager::getInstance()->Err("load file:%s failed, reason:%s", m_file, lua_tostring(m_pLua, -1));
}

// ��������(�Զ������������������)
void PLua::Call(const char* func, const char* fmt, va_list vl)
{
	lua_settop(m_pLua, 0); // ���ö�ջ
	lua_getglobal(m_pLua, func); // ��ȡ������ַ

	// ��������
	int narg = 0; // ��������
	bool isArgEnd = false; // �Ƿ�����б����
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
		case 'u': // user �Զ�������
		{
					  if (!PushPointer(va_arg(vl, void*), "CUser*"))
						  LogManager::getInstance()->Err("lua call func:%s error.", func);
					  break;
		}
		DEFINE_LUA_TYPE('t', "CScriptTest*")
		case '>': // ��������ֵ�ָ���
		{
					  isArgEnd = true;
		}
		default:
			LogManager::getInstance()->Err("lua call func:%s error. invalid argument", func);
		}
		++narg;
	}

	// ��������
	int nres = strlen(fmt); // ����ֵ����
	if (lua_pcall(m_pLua, narg, nres, 0) != 0) // ��������
	{
		LogManager::getInstance()->Err("lua call func:%s error. %s", func, lua_tostring(m_pLua, -1));
		return;
	}

	// ��ȡ����ֵ
	nres = -nres; // ���������
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

void PLua::GetValList(std::string& value, int argn, va_list vl, bool isFirstIn)
{
	if (argn < 1)
	{
		value = "";
		return;
	}
	std::string key = "";
	if (argn == 1)
	{
		if (isFirstIn)
		{
			//std::cout << "����1" << std::endl;
			isFirstIn = false;
			key = va_arg(vl, char*);
			GetVal(key.c_str(), value);
		}
		else
		{
			//std::cout << "����2" << std::endl;
			key = va_arg(vl, char*);
			lua_pushstring(m_pLua, key.c_str());
			lua_gettable(m_pLua, -2);
			if (!lua_isstring(m_pLua, -1))
			{
				LogManager::getInstance()->Err("%s is not a string", key.c_str());
				return;
			}
			value = lua_tostring(m_pLua, -1);
		}
	}
	else if (argn == 2)
	{
		if (isFirstIn)
		{
			//std::cout << "����3" << std::endl;
			isFirstIn = false;
			key = va_arg(vl, char*);
			lua_getglobal(m_pLua, key.c_str());
			if (!lua_istable(m_pLua, -1))
			{
				LogManager::getInstance()->Err("%s is not a table",key.c_str());
				return;
			}
			key = va_arg(vl, char*);
			lua_pushstring(m_pLua, key.c_str());
			lua_gettable(m_pLua, -2);
			if (!lua_isstring(m_pLua, -1))
			{
				LogManager::getInstance()->Err("%s is not a string", key.c_str());
				return;
			}
			value = lua_tostring(m_pLua, -1);
		}
		else
		{
			//std::cout << "����4" << std::endl;
			key = va_arg(vl, char*);
			lua_pushstring(m_pLua, key.c_str());
			lua_gettable(m_pLua, -2);
			if (!lua_istable(m_pLua, -1))
			{
				LogManager::getInstance()->Err("%s is not a table", key.c_str());
				return;
			}
			GetValList(value, --argn, vl, isFirstIn);
		}
	}
	else
	{
		if (isFirstIn)
		{
			//std::cout << "����5" << std::endl;
			isFirstIn = false;
			key = va_arg(vl, char*);
			lua_getglobal(m_pLua, key.c_str());
			key = va_arg(vl, char*);
			lua_pushstring(m_pLua, key.c_str());
			lua_gettable(m_pLua, -2);
			if (!lua_istable(m_pLua, -1))
			{
				LogManager::getInstance()->Err("%s is not a table", key.c_str());
				return;
			}
			GetValList(value, argn - 2, vl, isFirstIn);
		}
		else
		{
			//std::cout << "����6" << std::endl;
			lua_pushstring(m_pLua, key.c_str());
			lua_gettable(m_pLua, -2);
			if (!lua_istable(m_pLua, -1))
			{
				LogManager::getInstance()->Err("%s is not a table", key.c_str());
				return;
			}
			GetValList(value, --argn, vl, isFirstIn);
		}
	}
}

void PLua::StackDump() {
	lua_State * L = m_pLua;
	int i;
	int top = lua_gettop(L);

	for (i = 1; i <= top; i++) {  /* repeat for each level */
		int t = lua_type(L, i);
		switch (t) {
		case LUA_TSTRING:  /* strings */
			printf("`%s'", lua_tostring(L, i));
			break;
		case LUA_TBOOLEAN:  /* booleans */
			printf(lua_toboolean(L, i) ? "true" : "false");
			break;
		case LUA_TNUMBER:  /* numbers */
			printf("%g", lua_tonumber(L, i));
			break;
		default:  /* other values */
			printf("%s", lua_typename(L, t));
			break;
		}
		printf("  ");  /* put a separator */
	}
	printf("\n");     /* end the listing */
}
