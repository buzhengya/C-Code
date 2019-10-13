#include "rb_tree_test.h"
#include "..\common\define.h"
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

struct NodeInfo{
	RBTreeNode *	pNode;
	int32			nBlack;

	NodeInfo(RBTreeNode * node, int32 black)
	{
		pNode = node;
		if (!IsRed(node))
		{
			++black;
		}
		nBlack = black;
	}
};

bool CheckRBTree(RBTree * pTree)
{
	vector<NodeInfo> vecNode;
	RBTreeNode * pLeaf = pTree->pLeaf;
	if (pTree->pRoot != pLeaf)
	{
		ASSERT(!IsRed(pTree->pRoot)); // root must black
		vecNode.push_back({ pTree->pRoot, 0 });
	}

	int32 nMaxBlack = 0;
	while (!vecNode.empty())
	{
		vector<NodeInfo> vecTmp;
		for (auto node : vecNode)
		{
			auto * pNode = node.pNode;
			auto nBlack = node.nBlack;
			// check black length
			if (pNode->pLeft == pLeaf || pNode->pRight == pLeaf)
			{
				if (nMaxBlack == 0)
				{
					nMaxBlack = nBlack;
				}
				else if (nMaxBlack != nBlack)
				{
					LOG << "BLACK LENGTH NOT SATISFY. CUR : " << nBlack << " MAX : " << nMaxBlack << endl;
					PrintNodeDetail(pTree, pNode);
					return false;
				}
			}

			// check father and son is red
			if (pTree->pRoot != pNode && IsRed(pNode) && IsRed(pNode->pParent))
			{
				LOG << "SON AND FATHER IS RED." << endl;
				PrintNodeDetail(pTree, pNode);
				return false;
			}

			// check binary tree
			// son node
			if (pNode->pLeft != pLeaf)
			{
				ASSERT(pNode->nKey > pNode->pLeft->nKey);
				vecTmp.push_back({ pNode->pLeft, nBlack });
			}
			if (pNode->pRight != pLeaf)
			{
				ASSERT(pNode->nKey < pNode->pRight->nKey);
				vecTmp.push_back({ pNode->pRight, nBlack });
			}
		}

		vecNode.clear();
		vecNode.assign(vecTmp.begin(), vecTmp.end());
	}

	return true;
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

#define KEY_LEN 100000
void InitRand()
{
	nIndex = 0;
	vecRand.clear();
	srand(uint32(time(nullptr)));
	int32 nKeyLen = rand() % KEY_LEN;
	for (int32 i = 0; i < KEY_LEN + nKeyLen; ++i)
	{
		vecRand.push_back(i + KEY_LEN);
	}
	random_shuffle(vecRand.begin(), vecRand.end());
}

int32 GetRand()
{
	ASSERT(static_cast<uint32>(nIndex) < vecRand.size());
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

void InsertData()
{
	int32 nKey = GenKeyVal();
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
		InsertData();
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
