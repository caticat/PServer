#pragma once

/*
���Ͷ���
*/

#include <boost/shared_ptr.hpp>

typedef unsigned char uint8;
typedef char int8;
typedef unsigned short uint16;
typedef short int16;
typedef unsigned int uint32;
typedef int int32;
typedef unsigned long long uint64;
typedef long long int64;

/*
	���Ͷ���
*/
class CPLog;
typedef boost::shared_ptr<CPLog> log_t;
