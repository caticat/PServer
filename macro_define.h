#pragma once

/*
�򵥵���
*/
#define SINGTONE_GETINSTANCE(T) \
public:\
	static T* getInstance()\
	{\
		static T* _pInstance = NULL; \
		if (_pInstance == NULL)\
			_pInstance = new T; \
		return _pInstance; \
	}

#define SINGTONE(T) \
private:\
	T() {}\
SINGTONE_GETINSTANCE(T)

/*
�ű���������
*/
#define DEFINE_LUA_TYPE(C,T) \
	case C:\
	{\
		if (!PushPointer(va_arg(vl, void*), T))\
			LogManager::getInstance()->Err("lua call func:%s error.", func);\
		break;\
	}
