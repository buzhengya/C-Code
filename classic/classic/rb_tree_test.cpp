#include "rb_tree_test.h"
#include "define.h"
#include <map>
#include <vector>
#include <stack>
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

void PrintNodeStatus(RBTree * pTree, RBTreeNode * pNode);

// PreOrder 
int32 nMaxBlack = 0;
bool JudgeRBTree(RBTree * pTree, RBTreeNode * pNode)
{
	PrintNodeStatus(pTree, pNode);
	// root is black
	if (pTree->pRoot == pNode && IsRed(pNode))
	{
		LOG << "root is red" << endl;
		return false;
	}

	// red father and son
	if (pTree->pRoot != pNode && IsRed(pNode) && IsRed(pNode->pParent))
	{
		LOG << "father and son is red" << endl;
		return false;
	}

	// binary tree
	if (pNode->pLeft != pTree->pLeaf && pNode->pLeft->nKey >= pNode->nKey)
	{
		LOG << "not binary tree" << endl;
		return false;
	}

	if (pNode->pRight != pTree->pLeaf && pNode->pRight->nKey <= pNode->nKey)
	{
		LOG << "not binary tree" << endl;
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
			pTmp = pTmp->pParent;
		}

		if (nMaxBlack == 0)
		{
			nMaxBlack = nBlackNum;
		}
		else if (nMaxBlack != nBlackNum)
		{
			LOG << "max black : " << nMaxBlack << " cur black : " << nBlackNum << endl;
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
		cout << "left. key : " << pNode->pLeft->nKey << " " << GetColor(pNode->pLeft) << "  ";
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

bool PrePollTree(RBTree * pTree, DealNode func)
{
	stack<RBTreeNode*> stNode;
	if (pTree->pRoot != pTree->pLeaf)
	{
		stNode.push(pTree->pRoot);
	}

	RBTreeNode * pTop = nullptr;
	while (!stNode.empty())
	{
		pTop = stNode.top();
		if (!func(pTree, pTop))
		{
			PrintNodeStatus(pTree, pTop);
			return false;
		}

		if (pTop->pRight != pTree->pLeaf)
		{
			stNode.push(pTop->pRight);
		}

		if (pTop->pLeft != pTree->pLeaf)
		{
			stNode.push(pTop->pLeft);
		}
		stNode.pop();
	}

	return true;
}

bool InPollTree(RBTree * pTree, DealNode func)
{
	stack<RBTreeNode*> stNode;

	RBTreeNode * pTop = pTree->pRoot;
	while (pTop != pTree->pLeaf)
	{
		stNode.push(pTop);
		pTop = pTop->pLeft;
	}

	while (!stNode.empty())
	{
		pTop = stNode.top();
		if (!func(pTree, pTop))
		{
			return false;
		}
		stNode.pop();

		pTop = pTop->pRight;
		while (pTop != pTree->pLeaf)
		{
			stNode.push(pTop);
			pTop = pTop->pLeft;
		}
	}

	return true;
}

bool PostPollTree(RBTree * pTree, DealNode func)
{
	stack<RBTreeNode*> stNode;

	RBTreeNode * pLeaf = pTree->pLeaf;
	RBTreeNode * pTop = nullptr;
	RBTreeNode * pLast = nullptr;

	while (!stNode.empty())
	{
		// case 1 left and right is leaf
		// case 2 last is left and right is leaf
		// case 3 last is right
		if ((pTop->pLeft == pLeaf && pTop->pRight == pLeaf) || 
			(pLast == pTop->pLeft && pTop->pRight == pLeaf) || 
			pTop->pRight == pLast)
		{
			if (!func(pTree, pTop))
			{
				return false;
			}
			pLast = pTop;
			stNode.pop();
			continue;
		}

		if (pTop->pRight != pLeaf)
		{
			stNode.push(pTop->pRight);
		}

		if (pTop->pLeft != pLeaf)
		{
			stNode.push(pTop->pLeft);
		}
	}

	return true;
}

bool CheckRBTree(RBTree * pTree)
{
	InitBeforePoll();
	LOG << pTree->nSize << endl;
	return InPollTree(pTree, PrintNode) && InPollTree(pTree, JudgeRBTree);
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
	//ASSERT(CheckRBTree(pTree) == true);
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
	for (size_t i = 0; i < mapCache.size(); i++)
	{
		int32 nKey = GenKey();
		auto * pNode = RBTreeQuery(pTree, nKey);
		ASSERT(pNode == pTree->pLeaf);
	}
}

void TestRBTree(const int32 nCount)
{
	mapCache.clear();

	pTree = RBTreeInit();
	for (int32 i = 0; i < nCount; i++)
	{
		int32 nKey = GenKeyVal();
		InsertData(nKey);
		QueryData();
	}
	ASSERT(CheckRBTree(pTree) == true);
}
