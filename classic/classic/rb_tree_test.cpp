#include "rb_tree_test.h"
#include "define.h"
#include <map>
#include <vector>
#include <set>
#include <time.h>

using namespace std;

enum PollOrder
{
	PreOrder = 0,
	InOrder = 1,
	PostOrder = 2
};

RBTree * pTree;
vector<RBTreeNode*> vecNodeAddr;
bool CollectNodeAddr(RBTree * pTree, RBTreeNode * pNode)
{
	return true;
}

// PreOrder 
int32 nMaxBlack = 0;
bool JudgeRBTree(RBTree * pTree, RBTreeNode * pNode)
{
	// root is black
	if (pTree->pRoot == pNode && IsRed(pNode))
	{
		return false;
	}

	// red father and son
	if (pTree->pRoot != pNode && IsRed(pNode) && IsRed(pNode->pParent))
	{
		return false;
	}

	// binary tree
	if (pNode->pLeft != pTree->pLeaf && pNode->pLeft->nKey >= pNode->nKey)
	{
		return false;
	}

	if (pNode->pRight != pTree->pLeaf && pNode->pRight->nKey <= pNode->nKey)
	{
		return false;
	}

	// leaf black length
	if (pNode->pLeft == pTree->pLeaf || pNode->pRight == pTree->pLeaf)
	{
		RBTreeNode * pTmp = pNode;
		int32 nBlackNum = 2; // root black + leaf black
		while (pTmp != pTree->pRoot)
		{
			nBlackNum = IsRed(pTmp) ? nBlackNum : nBlackNum + 1;
		}

		if (nMaxBlack == 0)
		{
			nMaxBlack = nBlackNum;
		}
		else if (nMaxBlack != nBlackNum)
		{
			return false;
		}
	}

	return true;
}

char * GetColor(RBTreeNode * pNode)
{
	if (pNode->bRed)
	{
		return "Red";
	}

	return "Black";
}

void PrintNodeStatus(RBTree * pTree, RBTreeNode * pNode)
{
	if (pTree->pRoot == pNode)
	{
		cout << "root node!" << endl;
	}
	else
	{
		cout << "father. key : " << pNode->pParent->nKey << " " << GetColor(pNode->pParent) << endl;
	}

	cout << "self, key : " << pNode->nKey << " " << GetColor(pNode) << endl;

	if (pNode->pLeft != pTree->pLeaf)
	{
		cout << "left. key : " << pNode->pLeft->nKey << " " << GetColor(pNode->pLeft);
	}

	if (pNode->pRight != pTree->pLeaf)
	{
		cout << "right. key : " << pNode->pRight->nKey << " " << GetColor(pNode->pRight);
	}

	cout << endl;
}

int32 nLastKey = -1;
bool PrintNode(RBTree * pTree, RBTreeNode * pNode)
{
	cout << pNode->nKey << endl;

	int32 nTmp = nLastKey;
	nLastKey = pNode->nKey;
	if (nLastKey = -1)
	{
		nLastKey = pNode->nKey;
		return true;
	}
	return nTmp < nLastKey;
}

void InitBeforePoll()
{
	// print
	nLastKey = -1;

	// judge rbtree
	nMaxBlack = 0;

	// collect addr pointer
	vecNodeAddr.clear();
}

typedef bool(*DealNode)(RBTree *, RBTreeNode *);

bool PollRBTree(RBTree * pTree, DealNode func, PollOrder eOrder)
{
	if (pTree->pRoot == pTree->pLeaf)
	{
		return true;
	}

	InitBeforePoll();

	vector<RBTreeNode*> vecNode;
	vecNode.push_back(pTree->pRoot);
	RBTreeNode * pTop = pTree->pRoot;
	RBTreeNode * pLast = nullptr;

	while (!vecNode.empty())
	{
		pTop = vecNode[vecNode.size() - 1];

		// last is left 
		if (pTop->pLeft == pLast)
		{
			if (eOrder == InOrder)
			{
				if (func(pTree, pTop) == false)
				{
					PrintNodeStatus(pTree, pTop);
					return false;
				}
			}

			// right is leaf
			if (pTop->pRight == pTree->pLeaf)
			{
				pLast = pTop->pRight;
			}
			else
			{
				vecNode.push_back(pTop->pRight);
				pLast = pTop;
			}
		}
		// last is right
		else if (pTop->pRight == pLast)
		{
			if (eOrder == PostOrder)
			{
				if (func(pTree, pTop) == false)
				{
					PrintNodeStatus(pTree, pTop);
					return false;
				}
			}

			vecNode.pop_back();
			//deal cur call back
			pLast = pTop;
		}
		// last not left and right
		else
		{
			if (eOrder == PreOrder)
			{
				if (func(pTree, pTop) == false)
				{
					PrintNodeStatus(pTree, pTop);
					return false;
				}
			}

			if (pTop->pLeft == pTree->pLeaf)
			{
				pLast = pTop->pLeft;
			}
			else
			{
				vecNode.push_back(pTop->pLeft);
				pLast = pTop;
			}
		}
	}

	return true;
}

bool CheckRBTree(RBTree * pTree)
{
	return PollRBTree(pTree, JudgeRBTree, PreOrder);
}

int32 GenVal();
int32 GenKey();

map<int32, int32> mapCache;
int32 GenKeyVal()
{
	int32 nKey = GenKey();
	mapCache[nKey] = GenVal();
	return nKey;
}

int32 GenVal()
{
	srand(uint32(time(nullptr)));
	return rand();
}

int32 GenKey()
{
	int32 nKey = 0;
	srand(uint32(time(nullptr)));
	while (true)
	{
		nKey = rand();
		if (mapCache.find(nKey) == mapCache.end())
		{
			break;
		}
	}

	return nKey;
}

void InsertData(const int32 nKey)
{
	int32 nVal = mapCache[nKey];
	ASSERT(RBTreeInsert(pTree, nKey, nVal) == true);
	ASSERT(CheckRBTree(pTree) == true);
}

void DeleteData(const int32 nKey)
{
}

void QueryData()
{
	// check data in tree
	for (const auto & pairNode : mapCache)
	{
		auto * pNode = RBTreeQuery(pTree, pairNode.first);
		ASSERT(pNode != nullptr);
		ASSERT(pNode->nVal == pairNode.second);
	}

	// check data not in tree
	for (int32 i = 0; i < mapCache.size(); i++)
	{
		int32 nKey = GenKey();
		auto * pNode = RBTreeQuery(pTree, nKey);
		ASSERT(pNode == nullptr);
	}
}

void TestRBTree(const int32 nCount)
{
	pTree = RBTreeInit();
	for (int32 i = 0; i < nCount; i++)
	{
		int32 nKey = GenKeyVal();
		InsertData(nKey);
	}
}
