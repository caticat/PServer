#pragma once

#include <boost/function.hpp>

//typedef boost::singleton_pool<int,4,boost::default_user_allocator_new_delete,boost::details::pool::default_mutex,64> MemPool;
template<typename IndexType, typename ValType>
class CHashTable
{
private:
	struct SHashNode
	{
		IndexType id;
		SHashNode *pNext;
		ValType val;

		/*void *operator new(size_t)
		{
		return boost::singleton_pool<SHashNode,sizeof(SHashNode)>::malloc();
		}

		void operator delete(void *p)
		{
		boost::singleton_pool<SHashNode,sizeof(SHashNode)>::free(p);
		}*/

		SHashNode() :pNext(NULL)
		{
		}
		~SHashNode()
		{
		}
	};
public:
	CHashTable(int buckets)
	{
		int i = 0;
		for (; i < 20; i++)
		{
			if ((buckets <= 1 << (i + 1)) && (buckets >= 1 << i))
			{
				break;
			}
		}
		m_buckets = 1 << (i + 1);
		Init();
	}

	CHashTable()
	{
		m_buckets = 512;
		Init();
	}

	~CHashTable()
	{
		SHashNode *pNode;
		SHashNode *pNext;
		for (int i = 0; i < m_buckets; i++)
		{
			pNode = m_pHashTable[i];

			while (pNode != NULL)
			{
				pNext = pNode->pNext;
				delete pNode;
				pNode = pNext;
			}
		}
		delete[]m_pHashTable;
		m_pHashTable = NULL;
	}

	bool Find(IndexType index, ValType &val)

	{
		int i = index & m_modMask;
		SHashNode *pNode = m_pHashTable[i];
		while (pNode != NULL)
		{
			if (pNode->id == index)
			{
				val = pNode->val;
				return true;
			}
			pNode = pNode->pNext;
		}
		return false;;
	}

	void Insert(IndexType index, ValType &val)
	{
		int i = index & m_modMask;

		SHashNode *pNode = m_pHashTable[i];
		while (pNode != NULL)
		{
			if (pNode->id == index)
			{
				return;
			}
			pNode = pNode->pNext;
		}

		SHashNode *pNext = new SHashNode;
		pNext->id = index;
		pNext->val = val;
		pNext->pNext = m_pHashTable[i];
		m_pHashTable[i] = pNext;
	}

	void Erase(IndexType index, ValType &val)
	{
		int i = index & m_modMask;
		SHashNode **ppNode = m_pHashTable + i;
		while (*ppNode != NULL)
		{
			if ((*ppNode)->id == index)
			{
				SHashNode *pNext = (*ppNode)->pNext;
				val = (*ppNode)->val;
				delete *ppNode;
				*ppNode = pNext;
				return;
			}
			ppNode = &(*ppNode)->pNext;
		}
	}

	void Erase(IndexType index)
	{
		int i = index & m_modMask;
		SHashNode **ppNode = m_pHashTable + i;
		while (*ppNode != NULL)
		{
			if ((*ppNode)->id == index)
			{
				SHashNode *pNext = (*ppNode)->pNext;
				delete *ppNode;
				*ppNode = pNext;
				return;
			}
			ppNode = &(*ppNode)->pNext;
		}
	}

	void ForEach(boost::function<bool(IndexType, ValType)> fun)
	{
		SHashNode *pNode;

		for (int i = 0; i < m_buckets; i++)
		{
			pNode = m_pHashTable[i];
			while (pNode != NULL)
			{
				if (!fun(pNode->id, pNode->val))
				{
					return;
				}
				pNode = pNode->pNext;
			}
		}
	}


	void DelAll()
	{
		SHashNode *pNode;
		for (int i = 0; i < m_buckets; i++)
		{
			pNode = m_pHashTable[i];
			while (pNode != NULL)
			{
				delete pNode->val;
				pNode = pNode->pNext;
			}
		}
	}
private:
	SHashNode **m_pHashTable;
	int m_buckets;
	int m_modMask;

	void Init()

	{
		m_modMask = m_buckets - 1;
		m_pHashTable = new SHashNode*[m_buckets];
		memset(m_pHashTable, 0, sizeof(SHashNode*)*m_buckets);
	}
};
