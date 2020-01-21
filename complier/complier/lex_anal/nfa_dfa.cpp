#include "nfa_dfa.h"
#include "wrap_assert.h"

#include <set>
#include <queue>
using namespace std;

class CDfaState
{
public:
	CDfaState() : m_nState(0) {}
	CDfaState(int32 nState) : m_nState(nState) {}
	~CDfaState() {}

	bool IsContain(int32 nState) { return m_setState.find(nState) == m_setState.end(); }
	int32  GetState() const { return m_nState; }

	void SetState(int32 nState) { m_nState = nState; }

	void SetNfaState(const set<int32> setState) { m_setState = setState; }
	const set<int32>& GetNfaState() const { return m_setState; }

private:
	int32		m_nState;
	set<int32>	m_setState;
};

template<class T>
inline bool CTrie<T>::Insert(std::vector<T> vecList)
{
	bool bRet = false;
	CTrieNode<T>* pCur = &m_oRoot;
	for (auto& t : vecList)
	{
		ASSERT_CONTINUE_MSG(pCur != nullptr, "vecList size : " << vecList.size());

		if (pCur->m_mapTrie.find(t) == pCur->m_mapTrie.end())
		{
			bRet = true;
			CTrieNode<T>* pNew = new(CTrieNode<T>);
			ASSERT_CONTINUE(pNew != nullptr);
			pCur->m_mapTrie[t] = pNew;
		}

		pCur = pCur->m_mapTrie[t];
	}

	ASSERT_RETVAL_MSG(pCur != nullptr, false, "vecList size : " << vecList.size());
	if (bRet == true || pCur->m_bIsEnd == false)
	{
		pCur->m_bIsEnd = true;
		bRet = true;
	}

	return bRet;
}

template<class T>
void CTrie<T>::Clear()
{
	queue<CTrieNode<T>*> queNode;
	for (auto& pairNode : m_oRoot.m_mapTrie)
	{
		queNode.push(pairNode.second);
	}

	while (!queNode.empty())
	{
		CTrieNode<T>* pNode = queNode.front();
		queNode.pop();
		if (pNode == nullptr)
		{
			continue;
		}

		for (auto& pairNode : pNode->m_mapTrie)
		{
			queNode.push(pairNode.second);
		}
		delete pNode;
	}
}

bool CNfa2Dfa::Init(const CCell& oCell)
{
	LOG_INFO << "init nfa to dfa...";
	m_nNfaStart = oCell.GetStart();
	m_nNfaEnd = oCell.GetEnd();

	m_setInput = oCell.GetAllInput();

	LOG_INFO << "begin calculate nfa edge...";
	_CalNfaEdge(oCell);
	LOG_INFO << "success calculate nfa edge...";

	LOG_INFO << "begin calculate state closure..";
	for (auto& pairEdge : m_mapNfaEdge)
	{
		_CalSameState(pairEdge.first);
	}
	LOG_INFO << "success calculate state closure..";


	LOG_INFO << "success init nfa to dfa...";
	return true;
}

bool CNfa2Dfa::Nfa2Dfa()
{
	// clear
	m_oCell.Clear();
	m_oTrie.Clear();

	// 根据 nfa start state 生成 dfa start state
	CDfaState oStart(m_nState++);
	oStart.SetNfaState(_GetSameState(m_nNfaStart));
	ASSERT_RETVAL(m_oTrie.Insert(vector<int32>(oStart.GetNfaState().begin(), oStart.GetNfaState().end())), false);

	m_oCell.SetStart(oStart.GetState());
	if (oStart.IsContain(m_nNfaEnd))
	{
		m_oCell.InsertEnd(oStart.GetState());
	}

	queue<CDfaState> queState;
	queState.push(oStart);

	// 从初始状态开始逐个探索有效的组合
	while (!queState.empty())
	{
		auto & oTop = queState.front();
		queState.pop();

		// 从该状态的所有NFA子状态 寻找所有的输入 对应的输出
		for (auto szInput : m_setInput)
		{
			set<int32> setState;
			for (auto itNfaState : oTop.GetNfaState())
			{
				auto& vecEdge = m_mapNfaEdge[itNfaState];
				for (auto& oEdge : vecEdge)
				{
					if (oEdge.GetInput() == szInput)
					{
						setState.emplace(oEdge.GetEnd());
					}
				}
			}

			// 如果该输入对应的 状态集 不为空且不存在
			if (!setState.empty()&& m_oTrie.Insert(vector<int32>(setState.begin(), setState.end())))
			{
				CDfaState oNew(m_nState++);
				oNew.SetNfaState(setState);
				queState.push(oNew);
				m_oCell.PushEdge(oTop.GetState(), oNew.GetState(), szInput);
			}
		}
	}

	return true;
}

std::set<int32> CNfa2Dfa::_CalSameState(int32 nState)
{
	set<int32> setState;
	queue<int32> queState;
	queState.push(nState);

	while (!queState.empty())
	{
		int32 nState = queState.front();
		queState.pop();
		setState.insert(nState);

		ASSERT_RETVAL_MSG(m_mapNfaEdge.find(nState) != m_mapNfaEdge.end(), setState, "");
		auto& vecEdge = m_mapNfaEdge[nState];
		for (auto & edge : vecEdge)
		{
			if (edge.GetInput() == EmptyInput && setState.find(edge.GetEnd()) == setState.end())
			{
				setState.emplace(nState);
				queState.push(nState);
			}
		}
	}

	return setState;
}

void CNfa2Dfa::_CalNfaEdge(const CCell& oCell)
{
	auto& vecEdge = oCell.GetEdge();
	for (const auto& edge : vecEdge)
	{
		int32 nStart = edge.GetStart();

		auto& vecStateEdge = m_mapNfaEdge[nStart];
		vecStateEdge.push_back(edge);
	}
}

const std::set<int32>& CNfa2Dfa::_GetSameState(int32 nState)
{
	return m_mapSameState[nState];
}

std::set<int32> CNfa2Dfa::_GetAllSameState(std::set<int32> setInput)
{
	set<int32> setState;
	for (auto nState : setInput)
	{
		auto& setTmp = _GetSameState(nState);
		for (auto nTmp : setTmp)
		{
			setState.insert(nTmp);
		}
	}

	return setState;
}
