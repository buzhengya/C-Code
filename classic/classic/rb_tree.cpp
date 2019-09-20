#include "rb_tree.h"
#include <vector>
#include <assert.h>



bool IsRed(RBTreeNode * pNode)
{
	return pNode->bRed;
}

void SetRed(RBTreeNode * pNode)
{
	pNode->bRed = true;
}

void SetBlack(RBTreeNode * pNode)
{
	pNode->bRed = false;
}

bool IsLeaf(RBTree * pTree, RBTreeNode * pNode)
{
	return pNode->pLeft == pTree->pLeaf && pNode->pRight == pTree->pLeaf;
}



void LeftRotate(RBTree * pTree, RBTreeNode * pNode)
{
	RBTreeNode * pRight = pNode->pRight;

	// right son 's left son
	if (pRight->pLeft != pTree->pLeaf)
	{
		pRight->pLeft->pParent = pNode;
	}
	pNode->pRight = pRight->pLeft;

	//right son
	pRight->pParent = pNode->pParent;
	pRight->pLeft = pNode;

	// self
	pNode->pParent = pRight;

	// father
	if (pNode == pTree->pRoot)
	{
		pTree->pRoot = pRight;
	}
	else if (pRight->pParent->pLeft == pNode) // left node
	{
		pRight->pParent->pLeft = pRight;
	}
	else // right node
	{
		pRight->pParent->pRight = pRight;
	}
}

void RightRotate(RBTree * pTree, RBTreeNode * pNode)
{
	RBTreeNode * pLeft = pNode->pLeft;

	// left son 's right son
	if (pLeft->pRight != pTree->pLeaf)
	{
		pLeft->pRight->pParent = pNode;
	}
	pNode->pLeft = pLeft->pRight;

	// left son
	pLeft->pParent = pNode->pParent;
	pLeft->pRight = pNode;

	// self
	pNode->pParent = pLeft;

	// father
	if (pNode == pTree->pRoot)
	{
		pTree->pRoot = pLeft;
	}
	else if (pLeft->pParent->pLeft == pNode) // left node
	{
		pLeft->pParent->pLeft = pLeft;
	}
	else // right node
	{
		pLeft->pParent->pRight = pLeft;
	}
}

RBTree * RBTreeInit()
{
	RBTree * pTree = new RBTree;
	pTree->nSize = 0;

	RBTreeNode * pNode = new RBTreeNode;
	SetBlack(pNode);
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

// 0. 空树
// 1. 寻找插入点 寻找node (nKey < node.nKey && node.pLeft == pLeaf) || (nKey
// 2. 设置红色进行调整
bool RBTreeInsert(RBTree * pTree, int32 nKey, int32 nVal)
{
	RBTreeNode * pNode = pTree->pRoot;
	bool bIsLeft = false;
	while (pNode != pTree->pLeaf && pNode->nKey != nKey)
	{
		if (nKey < pNode->nKey && pNode->pLeft == pTree->pLeaf)
		{
			bIsLeft = true;
			break;
		}

		if (nKey > pNode->nKey && pNode->pRight == pTree->pLeaf)
		{
			bIsLeft = false;
			break;
		}
	}

	if (pNode != pTree->pLeaf && pNode->nKey == nKey)
	{
		return false;
	}

	RBTreeNode * pTmp = new(RBTreeNode);
	SetRed(pTmp);
	pTmp->nKey = nKey;
	pTmp->nVal = nVal;
	pTmp->pLeft = pTree->pLeaf;
	pTmp->pRight = pTree->pLeaf;

	// 0. 空树
	if (pTree->pRoot == pTree->pLeaf)
	{
		pTree->pRoot = pTmp;
		SetBlack(pTmp);
		return true;
	}


	pTmp->pParent = pNode;
	// 1.1 node 左节点
	if (bIsLeft == true)
	{
		pNode->pLeft = pTmp;
	}
	else // 1.2 node 右节点
	{
		pNode->pRight = pTmp;
	}
	pNode = pTmp;

	// 开始调整树
	while (pNode != pTree->pRoot && IsRed(pNode) && IsRed(pNode->pParent))
	{
		// father is left
		if (pNode->pParent == pNode->pParent->pParent->pLeft)
		{
			pTmp = pNode->pParent->pParent->pRight; //uncle
			if (IsRed(pTmp))
			{
				SetBlack(pNode->pParent);
				SetBlack(pTmp);
				SetRed(pTmp->pParent);
				pNode = pTmp->pParent;
			}
			else
			{
				// node is father right
				if (pNode == pNode->pParent->pRight)
				{
					// left rotate node father
					LeftRotate(pTree, pNode->pParent);
					pNode = pNode->pLeft;
				}

				SetBlack(pNode->pParent);
				SetRed(pNode->pParent->pParent);
				LeftRotate(pTree, pNode->pParent->pParent);
				// father is black exit
			}
		}
		else // father is right
		{
			pTmp = pNode->pParent->pParent->pLeft; //uncle
			if (IsRed(pTmp))
			{
				SetBlack(pNode->pParent);
				SetBlack(pTmp);
				SetRed(pTmp->pParent);
				pNode = pTmp->pParent;
			}
			else
			{
				// node is father left
				if (pNode == pNode->pParent->pLeft)
				{
					RightRotate(pTree, pNode->pParent);
					pNode = pNode->pRight;
				}

				SetBlack(pNode->pParent);
				SetRed(pNode->pParent->pParent);
				RightRotate(pTree, pNode->pParent->pParent);
				// father is black exit
			}
		}
	}

	SetBlack(pTree->pRoot);

	return true;
}

bool RBTreeDelete(RBTree * pTree, int32 nKey)
{
	return false;
}

RBTreeNode * RBTreeQuery(RBTree * pTree, int32 nKey)
{
	RBTreeNode * pNode = pTree->pRoot;
	while (pNode != pTree->pLeaf)
	{
		if (pNode->nKey == nKey)
		{
			break;
		}
		else if (nKey < pNode->nKey)
		{
			pNode = pNode->pLeft;
		}
		else
		{
			pNode = pNode->pRight;
		}
	}

	return pNode;
}
