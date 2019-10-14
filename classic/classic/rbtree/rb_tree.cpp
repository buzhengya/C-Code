#include "rb_tree.h"
#include <vector>
#include "rb_tree_test.h"
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
	pTree->pLeaf = pNode;

	pTree->pRoot = pTree->pLeaf;
	return pTree;
}

void SwapNodeColor(RBTreeNode * pNode1, RBTreeNode * pNode2)
{
	bool bRed = pNode1->bRed;
	pNode1->bRed = pNode2->bRed;
	pNode2->bRed = bRed;
}

// 0. 空树
// 1. 寻找插入点 寻找node (nKey < node.nKey && node.pLeft == pLeaf) || (nKey
// 2. 设置红色进行调整
bool RBTreeInsert(RBTree * pTree, int32 nKey, int32 nVal)
{
	RBTreeNode * pTmp = new(RBTreeNode);
	SetRed(pTmp);
	pTmp->nKey = nKey;
	pTmp->nVal = nVal;
	pTmp->pLeft = pTree->pLeaf;
	pTmp->pRight = pTree->pLeaf;

	return RBTreeInsert(pTree, pTmp);
}

bool RBTreeInsert(RBTree* pTree, RBTreeNode* pTmp)
{
	RBTreeNode * pNode = pTree->pRoot;
	bool bIsLeft = false;
	while (pNode != pTree->pLeaf && pNode->nKey != pTmp->nKey)
	{
		if (pTmp->nKey < pNode->nKey)
		{
			if (pNode->pLeft == pTree->pLeaf)
			{
				bIsLeft = true;
				break;
			}
			pNode = pNode->pLeft;
		}

		if (pTmp->nKey > pNode->nKey)
		{
			if (pNode->pRight == pTree->pLeaf)
			{
				bIsLeft = false;
				break;
			}
			pNode = pNode->pRight;
		}
	}

	if (pNode != pTree->pLeaf && pNode->nKey == pTmp->nKey)
	{
		return false;
	}

	// 0. 空树
	if (pTree->pRoot == pTree->pLeaf)
	{
		pTree->pRoot = pTmp;
		SetBlack(pTmp);
		++pTree->nSize;
		return true;
	}

	LOG << "details for node before insert." << std::endl;
	PrintNodeDetail(pTree, pNode);

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
			if (pTmp != pTree->pLeaf && IsRed(pTmp))
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
				RightRotate(pTree, pNode->pParent->pParent);
				// father is black exit
			}
		}
		else // father is right
		{
			pTmp = pNode->pParent->pParent->pLeft; //uncle
			if (pTmp != pTree->pLeaf && IsRed(pTmp))
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
				LeftRotate(pTree, pNode->pParent->pParent);
				// father is black exit
			}
		}
	}

	SetBlack(pTree->pRoot);

	++pTree->nSize;
	return true;
}

bool RBTreeDelete(RBTree * pTree, int32 nKey)
{
	// find delete node
	RBTreeNode * pNode = RBTreeQuery(pTree, nKey);
	if (pNode == pTree->pLeaf)
	{
		return false;
	}

	RBTreeNode * pLeaf = pTree->pLeaf;

	// find node 's next
	RBTreeNode * pTmp = pNode->pRight; // delete node 's son
	RBTreeNode * pLast = pNode; // to delete

	if (pNode->pRight == pLeaf) //right is leaf
	{
		pLast = pNode;
		pTmp = pNode->pLeft;
	}
	else if (pNode->pLeft == pLeaf) // left is leaf 
	{
		pLast = pNode;
		pTmp = pNode->pRight;
	}
	else
	{
		ASSERT(pNode->pLeft != pLeaf && pNode->pRight != pLeaf);
		pLast = pNode->pRight;
		pTmp = pLast->pLeft;
		while (pTmp != pLeaf)
		{
			pLast = pTmp;
			pTmp = pTmp->pLeft;
		}
		pTmp = pLast->pRight;
	}

	// swap last and node
	if (pLast != pNode)
	{
		int64 nTmpKey = pNode->nKey;
		int32 nTmpVal = pNode->nVal;

		pNode->nKey = pLast->nKey;
		pNode->nVal = pLast->nVal;

		pLast->nKey = nTmpKey;
		pLast->nVal = nTmpVal;
	}

	ASSERT(pLast->pLeft == pLeaf || pLast->pRight == pLeaf);
	ASSERT(pLast->nKey == nKey);

	LOG << "details for delete." << std::endl;
	PrintNodeDetail(pTree, pLast);

	// 删除 pLast前的调整
	// root node
	if (pLast == pTree->pRoot)
	{
		if (pTmp != pLeaf)
		{
			pTmp->pParent = pLeaf;
		}
		pTree->pRoot = pTmp;
	}
	else
	{
		pNode = pLast->pParent;
		// left son
		if (pNode->pLeft == pLast)
		{
			pNode->pLeft = pTmp;
		}
		// right son
		if (pNode->pRight == pLast)
		{
			pNode->pRight = pTmp;
		}

		pTmp->pParent = pNode;
	}

	// 如果被删除节点是红色 || 被删节点是根节点
	if (IsRed(pLast) || pTree->pRoot == pLeaf)
	{
		delete pLast;
		--pTree->nSize;
		return true;
	}
	delete pLast;

	pNode = pTmp;
	// 开始调整树 pNode有额外黑色 pNode可能为leaf节点
	while (!IsRed(pNode) && pNode != pTree->pRoot)
	{
		// pNode is left
		if (pNode == pNode->pParent->pLeft)
		{
			pTmp = pNode->pParent->pRight; //brother
			// case 1 brother is red
			if (IsRed(pTmp))
			{
				SwapNodeColor(pTmp, pTmp->pParent);
				LeftRotate(pTree, pTmp->pParent);
				continue;
			}
			
			// case 2 brother is black brother 's all son is black
			if (!IsRed(pTmp->pLeft) && !IsRed(pTmp->pRight))
			{
				// set brother red and parent is two black
				SetRed(pTmp);
				pNode = pNode->pParent;
				continue;
			}

			// case 3 brother is black and brother 's right is black(left must red)
			if (!IsRed(pTmp->pRight))
			{
				SwapNodeColor(pTmp, pTmp->pLeft);
				RightRotate(pTree, pTmp);
				pTmp = pTmp->pParent;
			}

			// case 4 brother is black and brother 's right is red
			if (IsRed(pTmp->pRight))
			{
				ASSERT(!IsRed(pTmp));
				SetBlack(pTmp->pRight);
				SwapNodeColor(pTmp, pTmp->pParent);
				LeftRotate(pTree, pTmp->pParent);
				ASSERT(!IsRed(pTmp->pLeft) && !IsRed(pTmp->pRight));
				ASSERT(!IsRed(pNode->pParent) && !IsRed(pNode->pParent->pParent->pRight));
				break;
			}
		}
		else  // pNode is right
		{
			// case 1 brother is red 
			pTmp = pNode->pParent->pLeft;
			if (IsRed(pTmp))
			{
				SwapNodeColor(pTmp, pTmp->pParent);
				RightRotate(pTree, pTmp->pParent);
				continue; // 此时 pTmp非彼pTmp 旋转后的pTmp????
			}
			
			// case 2 brother is black and brother 's all black
			if (!IsRed(pTmp->pLeft) && !IsRed(pTmp->pRight))
			{
				SetRed(pTmp);
				pNode = pNode->pParent;
				continue;
			}

			// case 3 brother is black and left son is black ( right son must red)
			if (!IsRed(pTmp->pLeft))
			{
				SwapNodeColor(pTmp, pTmp->pRight);
				LeftRotate(pTree, pTmp);
				pTmp = pTmp->pParent;
			}

			// case 4 brother is black and left son is red
			if (IsRed(pTmp->pLeft))
			{
				SetBlack(pTmp->pLeft);
				SwapNodeColor(pTmp, pTmp->pParent);
				RightRotate(pTree, pTmp->pParent);
				ASSERT(!IsRed(pNode->pParent) && !IsRed(pNode->pParent->pParent->pLeft));
				break;
			}
		}
	}

	SetBlack(pNode);
	--pTree->nSize;
	return true;
}

bool RBTreeDelete(RBTree* pTree, RBTreeNode* pNode)
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

RBTreeNode* MinNode(RBTreeNode* pNode, RBTreeNode* pLeaf)
{
	ASSERT(pNode != pLeaf);

	while (pNode->pLeft != pLeaf)
	{
		pNode = pNode->pLeft;
	}
	return pNode;
}

RBTreeNode* NextNode(RBTreeNode* pNode, RBTreeNode* pLeaf)
{
	ASSERT(pNode != pLeaf);
	if (pNode->pRight != pLeaf)
	{
		return MinNode(pNode->pRight, pLeaf);
	}
	else if (pNode->pParent != nullptr && pNode->pParent != pLeaf)
	{
		return pNode->pParent;
	}

	return nullptr;
}
