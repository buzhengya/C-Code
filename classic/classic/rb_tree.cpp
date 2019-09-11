#include "rb_tree.h"
#include <vector>
#include <assert.h>

using namespace std;

inline bool IsRed(RBTreeNode * pNode)
{
	return pNode->bRed;
}

inline bool DeleteNode(RBTree * pTree, RBTreeNode * pNode)
{

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
	if (pTree->pRoot == pTree->pLeaf)
	{
		return;
	}

	vector<RBTreeNode*> vecNode;
	RBTreeNode * pTop = pTree->pRoot;
	while (pTop != pTree->pLeaf)
	{
		vecNode.push_back(pTop);
		pTop = pTop->pLeft;
	}

	RBTreeNode * pLast = pTop->pLeft;
	while (!vecNode.empty())
	{
		pTop = vecNode[vecNode.size() - 1];
		vecNode.pop_back();

		if (pTop->pLeft == pLast)
		{
			pTo
		}
		else if (pTop->pRight == pLast)
		{

		}
		else
		{
			assert(false);
		}
	}

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
