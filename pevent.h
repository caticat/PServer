#pragma once

// �¼�����
enum EventType
{
	Eve_Test,
	Eve_Test1,
};

// �¼����� ��������
struct PEvent
{
	PEvent(EventType tp) : type(tp) {}
	EventType type;
};

struct PEvent2 : public PEvent
{
	PEvent2(EventType tp) : PEvent(tp) {}
	int data;
};
