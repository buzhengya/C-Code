#pragma once
#include "..\common\define.h"

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

bool IsRed(RBTreeNode * pNode);

RBTree * RBTreeInit();

bool RBTreeInsert(RBTree * pTree, int32 nKey, int32 nVal);

bool RBTreeDelete(RBTree * pTree, int32 nKey);

RBTreeNode * RBTreeQuery(RBTree * pTree, int32 nKey);
