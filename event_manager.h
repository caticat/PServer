#pragma once

/*
	�¼�ϵͳ
*/

#include "macro_define.h"
#include "pevent.h"
#include <map>
#include <list>
#include <boost/function.hpp>

// �����ж���ȵķ�ʽ����ʹ�ã����ǲ�ͬ����ķ���Ҳ�ᱻע���
//typedef boost::function<void(const PEvent&)> eveFunc_t; // boost���¼�
//typedef void(*eveFuncEqual_t)(const PEvent&); // �ж�ָ���Ƿ������
//bool operator==(eveFunc_t fl, eveFunc_t fr)
//{
//	eveFuncEqual_t fel = (eveFuncEqual_t)(fl.functor.obj_ptr);
//	eveFuncEqual_t fer = (eveFuncEqual_t)(fr.functor.obj_ptr);
//	return (fel == fer);
//}

// ʱ�����
class EventManager
{
private:
	typedef boost::function<void(const PEvent&)> eveFunc_t; // boost���¼�
	//typedef void(*eveFunc_t)(const PEvent&); // ��ͨȫ�ֺ������¼�
	typedef std::list<eveFunc_t> eveList_t;
	typedef std::map<EventType, eveList_t> eveMap_t;
	typedef eveList_t::iterator eveListIt_t;
	typedef eveMap_t::iterator eveMapIt_t;

	SINGTONE(EventManager)

public:
	void Push(EventType type, eveFunc_t eveFunc)
	{
		eveList_t* pList = GetList(type);
		if (pList != NULL)
		{
			pList->push_back(eveFunc);
			return;
		}
		eveList_t eveList;
		std::pair<eveMapIt_t, bool> mapPair = m_eveMap.insert(std::make_pair(type, eveList));
		if (!mapPair.second)
			return;
		mapPair.first->second.push_back(eveFunc);
	}

	void Pop(EventType type, eveFunc_t eveFunc = NULL)
	{
		eveMapIt_t mapIt = m_eveMap.begin();
		for (; mapIt != m_eveMap.end(); ++mapIt)
		{
			if (mapIt->first == type)
				break;
		}
		if (mapIt == m_eveMap.end())
			return;
		if (eveFunc == NULL)
		{
			m_eveMap.erase(mapIt);
			return;
		}
		eveList_t& eveList = mapIt->second;
		for (eveListIt_t listIt = eveList.begin(); listIt != eveList.end(); ++listIt)
		{
			if ((*listIt).functor.obj_ptr == eveFunc.functor.obj_ptr) // �������ͳһɾ�����ǰ��� ��->�����жϵģ����� ����->�����жϵ�
				listIt = eveList.erase(listIt);
			//if ((*listIt) == eveFunc) // ����ж��Ȳ�����
			//	listIt = eveList.erase(listIt);
		}
	}

	void Launch(const PEvent& eve)
	{
		eveList_t* pList = GetList(eve.type);
		if (pList == NULL)
			return;
		for (eveListIt_t it = pList->begin(); it != pList->end(); ++it)
			(*it)(eve);
	}

private:
	eveList_t* GetList(EventType type)
	{
		for (eveMapIt_t it = m_eveMap.begin(); it != m_eveMap.end(); ++it)
		{
			if (it->first == type)
				return &(it->second);
		}
		return NULL;
	}

private:
	eveMap_t m_eveMap;
};
