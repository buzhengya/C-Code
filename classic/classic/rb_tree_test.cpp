#include "rb_tree_test.h"
#include "define.h"
#include <map>
#include <vector>
#include <stack>
#include <set>
#include <time.h>
#include <algorithm>

using namespace std;

enum PollOrder
{
	PreOrder = 0,
	InOrder = 1,
	PostOrder = 2
};

RBTree * pTree;

// PreOrder 
int32 nMaxBlack = 0;
bool JudgeRBTree(RBTree * pTree, RBTreeNode * pNode)
{
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

bool PrintNodeStatus(RBTree * pTree, RBTreeNode * pNode)
{
	cout << "self, key : " << pNode->nKey << " " << GetColor(pNode) << "  ";

	if (pNode->pLeft != pTree->pLeaf)
	{
		cout << "left. key : " << pNode->pLeft->nKey << " " << GetColor(pNode->pLeft) << "  ";
	}

	if (pNode->pRight != pTree->pLeaf)
	{
		cout << "right. key : " << pNode->pRight->nKey << " " << GetColor(pNode->pRight);
	}

	cout << endl;
	return true;
}

bool PrintNodeDetail(RBTree * pTree, RBTreeNode * pNode)
{
	//father
	if (pTree->pRoot != pNode)
	{
		auto * pParent = pNode->pParent;
		if (pParent != pTree->pLeaf)
		{
			LOG << "---------------------------- father ----------------------------" << endl;
			PrintNodeStatus(pTree, pParent);
		}

		auto * pBrother = pParent->pLeft == pNode ? pParent->pRight : pParent->pLeft;
		if (pBrother != pTree->pLeaf)
		{
			LOG << "---------------------------- brother ----------------------------" << endl;
			PrintNodeStatus(pTree, pBrother);
		}
	}

	LOG << "---------------------------- self ----------------------------" << endl;
	PrintNodeStatus(pTree, pNode);

	if (pNode->pLeft != pTree->pLeaf)
	{
		LOG << "------------------------------- left son -------------------------------" << endl;
		PrintNodeStatus(pTree, pNode->pLeft);
	}
	if (pNode->pRight != pTree->pLeaf)
	{
		LOG << "------------------------------- right son -------------------------------" << endl;
		PrintNodeStatus(pTree, pNode->pRight);
	}
	return true;
}

void InitBeforePoll()
{
	// judge rbtree
	nMaxBlack = 0;
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
			LOG << "deal func failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
			PrintNodeDetail(pTree, pTop);
			return false;
		}
		stNode.pop();

		if (pTop->pRight != pTree->pLeaf)
		{
			stNode.push(pTop->pRight);
		}

		if (pTop->pLeft != pTree->pLeaf)
		{
			stNode.push(pTop->pLeft);
		}
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
			LOG << "deal func failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
			PrintNodeDetail(pTree, pTop);
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
				LOG << "deal func failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
				PrintNodeDetail(pTree, pTop);
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
	LOG << "begin check rbtree. tree size : " << pTree->nSize << endl;
	return InPollTree(pTree, JudgeRBTree);
}

int32 GetRand();
int32 GenKey();

map<int32, int32> mapCache;
set<int32> setKey;
int32 GenKeyVal()
{
	int32 nKey = GenKey();
	mapCache[nKey] = GetRand();
	setKey.insert(nKey);
	return nKey;
}

vector<int32> vecRand;
int32 nIndex;

void InitRand()
{
	nIndex = 0;
	vecRand.clear();
	for (int32 i = 0; i < 100000; ++i)
	{
		vecRand.push_back(i + 100000);
	}
	random_shuffle(vecRand.begin(), vecRand.end());
}

int32 GetRand()
{
	ASSERT(nIndex < vecRand.size());
	return vecRand[nIndex++];
}

int32 GetKey()
{
	// set ¿ÕÊ± å´»ú
	ASSERT(setKey.size() > 0);
	vector<int32> vecKey = vector<int32>(setKey.begin(), setKey.end());
	int32 nIndex = GetRand() % vecKey.size();
	return vecKey[nIndex];
}

void DeleteKey(int32 nKey)
{
	if (mapCache.find(nKey) != mapCache.end())
	{
		mapCache.erase(nKey);
	}

	if (setKey.find(nKey) != setKey.end())
	{
		setKey.erase(nKey);
	}
}

int32 GenKey()
{
	int32 nKey = 0;
	while (true)
	{
		nKey = GetRand();
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
	LOG << "insert key : " << nKey << endl;

	ASSERT(CheckRBTree(pTree) == true);
}

void DeleteData()
{
	int32 nKey = GetKey();

	// for debug 
	auto * pNode = RBTreeQuery(pTree, nKey);
	ASSERT(pNode != nullptr);


	ASSERT(RBTreeDelete(pTree, nKey) == true);
	DeleteKey(nKey);
	LOG << "delete key : " << nKey << endl;

	nKey = GenKey();
	ASSERT(RBTreeDelete(pTree, nKey) == false);
}

void QueryData()
{
	// check data in tree
	int32 nKey = GetKey();
	auto * pNode = RBTreeQuery(pTree, nKey);
	ASSERT(pNode != nullptr);
	ASSERT(pNode->nKey == nKey);

	// check data not in tree
	nKey = GenKey();
	pNode = RBTreeQuery(pTree, nKey);
	ASSERT(pNode == pTree->pLeaf);
}

void TestRBTree(const int32 nCount)
{
	mapCache.clear();
	InitRand();

	pTree = RBTreeInit();
	for (int32 i = 0; i < nCount; ++i)
	{
		int32 nKey = GenKeyVal();
		InsertData(nKey);
		ASSERT(CheckRBTree(pTree) == true);
		QueryData();
		if (GetRand() % 3 == 0)
		{
			DeleteData();
			ASSERT(CheckRBTree(pTree) == true);
		}
	}

	int32 nSize = mapCache.size();
	for (int32 i = 0; i < nSize; ++i)
	{
		DeleteData();
		ASSERT(CheckRBTree(pTree));
	}
}
