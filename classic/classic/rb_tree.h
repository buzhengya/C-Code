#pragma once
#include "define.h"

struct RBTreeNode
{
	RBTreeNode *	pParent;
	RBTreeNode *	pLeft;
	RBTreeNode *	pRight;
	int32			nKey;
	int32			nVal;
	bool			bRed;
};

struct RBTree
{
	RBTreeNode *	pRoot;
	RBTreeNode *	pLeaf;
	int32			nSize;
};

RBTree * RBTreeInit();

void RBTreeRelease(RBTree * pTree);

bool RBTreeInsert(RBTree * pTree, int32 nKey, int32 nVal);

bool RBTreeDelete(RBTree * pTree, int32 nKey);

RBTreeNode * RBTreeQuery(int32 nKey);
