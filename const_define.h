#pragma once

/*
	��������
*/

enum CONST_DEF
{
	PATH_LEN = 128, // ·������
	TIMER_TIMEOUT = 10, // ��ʱ�����ü��
	SLEEP_TIMEOUT = 1, // ˯��ʱ����
	MAX_CONN = 4096, // ������֧���������ֵ
	EPOLL_WAIT_TIMEOUT = 1000, // epoll_wait�ĳ�ʱʱ��
};

enum THREAD_DEF
{
	THREAD_PACKAGEDEAL = 0, // ��Ϣ�����̣߳�ͬʱ������Ϣ���أ�
	THREAD_TIMER = 1, // �źŴ����̣߳�ͬʱ����ʱ��ͬ����
	THREAD_SIGNAL = 2, // ��ʱ���߳�
	THREAD_NUM = 3, // ���߳�������߳�����
};

enum LOG_DEF
{
	LOG_NORMAL = 0, // ��ͨ��־
	LOG_WARN = 1, // ����
	LOG_ERROR = 2, // ����
	LOG_ALL = 3, // ȫ��
	LOG_NUM = 4, // ��־������
};

enum NETMSG_DEF
{
	NETMSG_MAX_LEN = 0xFFFF, // ��Ϣ�����
	NETMSG_HEAD_LEN = 4, // Э��ͷ����
	NETMSG_TYPE_BEGIN = 2, // ������ʼλ��
};

// ���ݿ�����
extern const int DB_CONN_TIMEOUT; // ���ӳ�ʱʱ��
extern const int DB_RECV_TIMEOUT; // ���س�ʱʱ��
