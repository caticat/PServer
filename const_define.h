#pragma once

/*
	��������
*/

enum CONST_DEF
{
	PATH_LEN = 128, // ·������
};

enum LOG_DEF
{
	LOG_NORMAL = 0, // ��ͨ��־
	LOG_WARN = 1, // ����
	LOG_ERROR = 2, // ����
	LOG_NUM = 3, // ��־������
};

// ���ݿ�����
extern const int DB_CONN_TIMEOUT; // ���ӳ�ʱʱ��
extern const int DB_RECV_TIMEOUT; // ���س�ʱʱ��
