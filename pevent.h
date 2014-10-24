#pragma once

// 事件类型
enum EventType
{
	Eve_Test,
	Eve_Test1,
};

// 事件参数 基本类型
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
