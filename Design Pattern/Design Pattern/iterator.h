#pragma once

#include<vector>

using namespace std;

template<class Item>
class IIterator
{
public:
	virtual void First() = 0;
	virtual void Next() = 0;
	virtual Item* CurrentItem() = 0;
	virtual bool IsDone() = 0;
};

template<class Item>
class CConcreteAggregate;

template<class Item>
class CConcretrIterator:public IIterator<Item>
{
private:
	int m_nCur;
	CConcreteAggregate<Item>* m_poAggre;
public:
	CConcretrIterator(CConcreteAggregate<Item>* poAggre)
	{
		m_poAggre = poAggre;
		m_nCur = 0;
	}

	void First()
	{
		m_nCur = 0;
	}

	void Next()
	{
		if(m_nCur<m_poAggre->Size())
		{
			m_nCur++;
		}
	}

	Item* CurrentItem()
	{
		if (m_nCur < m_poAggre->Size())
		{
			return &(*m_poAggre)[m_nCur];
		}
		else
		{
			return nullptr;
		}
	}
	bool IsDone()
	{
		return m_nCur >= m_poAggre->Size();
	}
};

template<class Item>
class IAggregate
{
public:
	virtual IIterator<Item>* CreateIterator() = 0;
	virtual Item& operator[](int nIndex) = 0;
	virtual int Size() = 0;
	virtual void push_back(Item Item) = 0;
};

template<class Item>
class CConcreteAggregate:public IAggregate<Item>
{
private:
	vector<Item> m_vecItem;
	int m_nCur;
public:
	IIterator<Item>* CreateIterator()
	{
		return new CConcretrIterator<Item>(this);
	}
	
	Item& operator[](int nIndex)
	{
		return m_vecItem[nIndex];
	}

	int Size()
	{
		return m_vecItem.size();
	}

	void push_back(Item Item)
	{
		m_vecItem.push_back(Item);
	}
};