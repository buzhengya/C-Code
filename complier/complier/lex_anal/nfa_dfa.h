#pragma once
#include "basic_data.h"

#include <map>
#include <set>

// TODO �����ݽṹ�ɸ��� CCell 
class CDfaCell
{
public:
	CDfaCell() :m_nStartState(0) {}
	CDfaCell(int32 nState) :m_nStartState(nState) {}
	~CDfaCell() {}
	void PushEdge(int32 nStart, int32 nEnd, char szInput) { m_vecEdge.emplace_back(nStart, nEnd, szInput); }
	void InsertEnd(int32 nEnd) { m_setEndState.emplace(nEnd); }
	void Clear()
	{
		m_setEndState.clear();
		m_vecEdge.clear();
	}

	void SetStart(int32 nState) { m_nStartState = nState; }

	int32 GetStart() const { return m_nStartState; }

	const std::set<int32> GetEnd() const { return m_setEndState; }
	const std::vector<CEdge> GetEdge() const { return m_vecEdge; }

private:
	int32 m_nStartState;
	std::set<int32> m_setEndState;
	std::vector<CEdge> m_vecEdge;
};

template<class T>
struct CTrieNode
{
	CTrieNode() : m_bIsEnd(false){}
	std::map<T, CTrieNode<T>*> m_mapTrie;
	bool m_bIsEnd;
};

template<class T>
class CTrie
{
public:
	CTrie() {}
	~CTrie() { Clear(); }

	bool Insert(std::vector<T> vecList);
	void Clear();

private:
	CTrieNode<T>	m_oRoot;
};

class CNfa2Dfa
{
public:
	CNfa2Dfa() {}
	~CNfa2Dfa() {}

	bool Init(const CCell & oCell);
	bool Nfa2Dfa();

private:
	// �հ�����
	std::set<int32> _CalSameState(int32 nState);

	// ���¼���nfa�� edge ���߹�ϵ���� �õ� m_mapNfaEdge
	void _CalNfaEdge(const CCell& oCell);

	// ��ȡ����״̬�ĵ�Ч״̬
	const std::set<int32>& _GetSameState(int32 nState);

	// ��ȡ���״̬�ĵ�Ч״̬
	std::set<int32> _GetAllSameState(std::set<int32> setInput);


	std::map<int32, std::set<int32>> m_mapSameState;
	// nfa �ı߹�ϵ
	std::map<int32, std::vector<CEdge>> m_mapNfaEdge;

	int32 m_nState;
	int32 m_nNfaStart;
	int32 m_nNfaEnd;
	std::set<char> m_setInput;

	CTrie<int32> m_oTrie;
	CDfaCell	m_oCell;
};

