#pragma once

/*
	ʱ�����
	��ʱ��ÿ����ø���
	�ⲿ���û�ȡʱ����Ϣ

	ע��
	��Ҫ������ʹ��
*/

#include "macro_define.h"
#include <time.h>

class TimeManager
{
	SINGTONE_GETINSTANCE(TimeManager)
private:
	TimeManager() : m_curTime(0) {}
public:
	void Sync() { m_curTime = time(NULL); }; // ͬ��ʱ��
	time_t GetTime() { return m_curTime; } // ��ȡ��ǰʱ��

private:
	time_t m_curTime; // ��ǰʱ��
};

/* ��ش����¼
#include <stdio.h>
#include <time.h>

void main()
{
	time_t sec;       //typedef long time_t
	struct tm * curTime;

	sec = time(NULL);           //��ȡʱ�䣬��1970.1.1�����ڵ�����,Ҳ����д�� time(&sec);
	curTime = localtime(&sec);  //��secת��Ϊ����ʱ�䣬����ʱ��ṹ��curTime�У�ע�ⷵ��ֵ��struct tm* ���͵�

	printf("asctime(curTime): %s\n", asctime(curTime));
	//asctime(struct tm*)������curTimeת���ɱ�׼ASCIIʱ���ʽ

	printf("ctime(curTime):   %s\n", ctime(&sec));
	//ctime(time_t*)ֱ�ӽ�secת���ɱ�׼ʱ���ʽ

	printf("%d-%d-%d %d:%d:%d\n", 1900 + curTime->tm_year, 1 + curTime->tm_mon,
		curTime->tm_mday, curTime->tm_hour, curTime->tm_min, curTime->tm_sec);
	//�ֶ����struct tm�е��ֶΣ������·ݼ�1�����Ҫ��1900��
	//sec��¼����1970�����ڵ�������Ϊʲô�Ǽ�1900�أ�����

	printf("\nsec = %ld\nmktime(curTime) = %ld\n", sec, mktime(curTime));
	//time_t mktime(struct tm*)��curTime����ת����1970�����ڵ�����
}
*/
