#include "rb_tree.h"
#include <vector>
#include <assert.h>

using namespace std;

enum PollOrder
{
	PreOrder = 0,
	InOrder = 1,
	PostOrder = 2
};

bool IsRed(RBTreeNode * pNode)
{
	return pNode->bRed;
}

bool IsLeaf(RBTree * pTree, RBTreeNode * pNode)
{
	return pNode->pLeft == pTree->pLeaf && pNode->pRight == pTree->pLeaf;
}

vector<RBTreeNode*> vecNodeAddr;
bool CollectNodeAddr(RBTree * pTree, RBTreeNode * pNode)
{
	
}

int32 nMaxBlack = 0;
int32 nCurBlack = 0;
bool JudgeRBTree(RBTree * pTree, RBTreeNode * pNode)
{
	// root is black

	// red father and son

	// binary tree

	// leaf black length
}

void PrintNodeStatus(RBTree * pTree, RBTreeNode * pNode)
{

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
	nCurBlack = 0;

	// collect addr pointer
	vecNodeAddr.clear();
}

typedef bool(*DealNode)(RBTree *, RBTreeNode *);

bool PollRBTree(RBTree * pTree, DealNode func, PollOrder eOrder)
{
	if (pTree->pRoot == pTree->pLeaf)
	{
		return;
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

RBTree * RBTreeInit()
{
	RBTree * pTree = new RBTree;
	pTree->nSize = 0;

	RBTreeNode * pNode = new RBTreeNode;
	pNode->bRed = false;
	pNode->pParent = nullptr;
	pNode->pLeft = nullptr;
	pNode->pRight = nullptr;

	pTree->pRoot = pTree->pLeaf;
	return pTree;
}

void RBTreeRelease(RBTree * pTree)
{
	// empty


}

bool RBTreeInsert(RBTree * pTree, int32 nKey, int32 nVal)
{
	return false;
}

bool RBTreeDelete(RBTree * pTree, int32 nKey)
{
	return false;
}

RBTreeNode * RBTreeQuery(int32 nKey)
{

}
