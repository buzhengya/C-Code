#pragma once
#include "define.h"
#include <vector>
#include <set>

const char EmptyInput = '@';

class CEdge
{
public:
	CEdge(int32 nStart, int32 nEnd, char szInput = EmptyInput) :
		m_nStartState(nStart), m_nEndState(nEnd), m_szInput(szInput) {}

	~CEdge() {}
	void ModifyStart(int32 nSrcState, int32 nDstState) 
	{ 
		m_nStartState = nSrcState == m_nStartState ? nDstState : m_nStartState; 
	}

	void ModifyEnd(int32 nSrcState, int32 nDstState)
	{
		m_nEndState = nSrcState == m_nEndState ? nDstState : m_nEndState;
	}

	int32 GetStart() const { return m_nStartState; }
	int32 GetEnd() const { return m_nEndState; }
	char GetInput() const { return m_szInput; }

private:
	char m_szInput; //��ʾ����״̬ת����Ҫ�������ַ�
	int32 m_nStartState; // ��ʼ״̬
	int32 m_nEndState; // ����״̬
};

class CCell
{
public:
	CCell() :m_nStartState(), m_nEndState(0) {}
	CCell(int32 nStart, int32 nEnd) : m_nStartState(nStart), m_nEndState(nEnd) {}
	~CCell() {}

	void PushEdge(int32 nStart, int32 nEnd, char szInput = EmptyInput)
	{
		m_vecEdge.emplace_back(nStart, nEnd, szInput);
	}

	void SetStart(int32 nStart) { m_nStartState = nStart; }
	void SetEnd(int32 nEnd) { m_nEndState = nEnd; }

	int32 GetStart() const { return m_nStartState; }
	int32 GetEnd() const { return m_nEndState; }

	void MergeEdge(CCell& oCell)
	{
		for (auto& it : oCell.m_vecEdge)
		{
			m_vecEdge.push_back(it);
		}
	}

	void Print() const;

	void SetAllInput(const std::set<char>& setAllInput) { m_setAllInput = setAllInput; }
	const std::set<char>& GetAllInput()const { return m_setAllInput; }
	const std::vector<CEdge>& GetEdge()const { return m_vecEdge; }

private:
	std::vector<CEdge> m_vecEdge; // ���еı�
	int32 m_nStartState; // 
	int32 m_nEndState; // �� regular expression to nfa ֻ��һ������״̬ �� nfa �� dfa ���ܻ�����������״̬
	std::set<char> m_setAllInput;
};


