#pragma once

/*
	宏定义
*/

#ifndef NULL
#define NULL 0
#endif

/*
简单单例
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
脚本参数传入
*/
#define DEFINE_LUA_TYPE(C,T) \
	case C:\
	{\
		if (!PushPointer(va_arg(vl, void*), T))\
			LogManager::getInstance()->Err("lua call func:%s error.", func);\
		break;\
	}

/*
释放指针
*/
#define RELEASE(p) {if(p!=NULL){delete p;p=NULL;}}
#define RELEASE_ARR(p) {if(p!=NULL){delete[] p;p=NULL;}}

/*
指针校验
*/
#define CONFIRM(p) {if(p==NULL){return;}}

/*
数据库连接创建
*/
#define GETDB \
DbConnection dbConn;\
DB* pDb = dbConn.GetDbConnection();\
if (pDb == NULL)\
{\
	LogManager::getInstance()->Err("获取连接数据库出错"); \
	return;\
}

/*
协议绑定
*/
#define PTL_BAND(PTL,FUNC) {PTL,boost::bind(&PackageDeal::FUNC,this,_1,_2)}
