#pragma once
#include<iostream>

#define RED true
#define BLACK false

template<class T>
class CRBNode
{
public:
	bool m_bColor;
	T m_tKey;
	CRBNode* m_pLeft;
	CRBNode* m_pRight;
	CRBNode* m_pParent;
	CRBNode(T tKey, bool bColor, CRBNode* pParent = nullptr, CRBNode* pLeft = nullptr, CRBNode* pRight = nullptr)
	{
		m_bColor = bColor;
		m_tKey = tKey;
		m_pLeft = pLeft;
		m_pRight = pRight;
		m_pParent = pParent;
	}
};

template<class T>
class CRBTree
{
private:
	CRBNode<T>* m_Root;

private:
	void LeftRotate(CRBNode<T>* pCur);
	void RightRotate(CRBNode<T>* pCur);
	void print(CRBNode<T>* pCur);
	void RecursiveDel(CRBNode<T>* pCur);
public:
	CRBTree();
	~CRBTree();
	CRBNode<T>* GetRoot();
	void PreOrder(CRBNode<T>* pCur);
	void MedOrder(CRBNode<T>* pCur);
	void PostOrder(CRBNode<T>* pCur);
	void Insert(T tKey);
	void Insert(CRBNode<T>* pNode);
	void InsertFixUp(CRBNode<T>* pCur);
};

#include "red_black_tree.h"


template<class T>
void CRBTree<T>::LeftRotate(CRBNode<T>* pCur)//左旋 pCur表示当前节点 pRight表示pCur的右子节点 pParent表示pCur的父节点
{
	CRBNode<T>* pRight = pCur->m_pRight;
	CRBNode<T>* pParent = pCur->m_pParent;

	//对当前节点的操作
	pCur->m_pRight = pRight->m_pLeft;
	pCur->m_pParent = pRight;

	//右子节点的操作
	pRight->m_pLeft = pCur;
	pRight->m_pParent = pParent;

	//对父节点操作
	if (pParent == nullptr)//为空则更改根节点
	{
		m_Root = pRight;
	}
	else
	{
		if (pCur == pParent->m_pLeft)//如果原节点是父节点的左子节点
		{
			pParent->m_pLeft = pRight;
		}
		else
		{
			pParent->m_pRight = pRight;
		}
	}
}

template<class T>
void CRBTree<T>::RightRotate(CRBNode<T>* pCur)//右旋 x表示当前节点 y表示x的左子节点 p表示x的父节点
{
	CRBNode<T>* pLeft = pCur->m_pLeft;
	CRBNode<T>* pParent = pCur->m_pParent;

	//对当前节点的操作
	pCur->m_pLeft = pLeft->m_pRight;
	pCur->m_pParent = pLeft;

	//左子节点的操作
	pLeft->m_pRight = pCur;
	pLeft->m_pParent = pParent;

	//对父节点操作
	if (pParent == nullptr)//为空则更改根节点
	{
		m_Root = pLeft;
	}
	else
	{
		if (pCur == pParent->m_pLeft)//如果原节点是父节点的左子节点
		{
			pParent->m_pLeft = pLeft;
		}
		else
		{
			pParent->m_pRight = pLeft;
		}
	}
}

template<class T>
void CRBTree<T>::print(CRBNode<T>* pCur)
{
	std::cout << pCur->m_tKey << "	";
}

template<class T>
void CRBTree<T>::RecursiveDel(CRBNode<T>* pCur)
{
	if (pCur == nullptr) return;
	RecursiveDel(pCur->m_pLeft);
	RecursiveDel(pCur->m_pRight);
	delete pCur;
}

template<class T>
CRBTree<T>::CRBTree()
{
	m_Root = nullptr;
}

template<class T>
CRBTree<T>::~CRBTree()
{
	RecursiveDel(m_Root);
}

template<class T>
CRBNode<T>* CRBTree<T>::GetRoot()
{
	return m_Root;
}

template<class T>
void CRBTree<T>::PreOrder(CRBNode<T>* pCur)
{
	if (pCur == nullptr) return;
	print(pCur);
	PreOrder(pCur->m_pLeft);
	PreOrder(pCur->m_pRight);
}

template<class T>
void CRBTree<T>::MedOrder(CRBNode<T>* pCur)
{
	if (pCur == nullptr) return;
	MedOrder(pCur->m_pLeft);
	print(pCur);
	MedOrder(pCur->m_pRight);
}

template<class T>
void CRBTree<T>::PostOrder(CRBNode<T>* pCur)
{
	if (pCur == nullptr) return;
	PostOrder(pCur->m_pLeft);
	PostOrder(pCur->m_pRight);
	print(pCur);
}

template<class T>
void CRBTree<T>::Insert(T tKey)
{
	CRBNode<T>* poNode = new CRBNode<T>(tKey, RED);
	if (poNode != nullptr)
	{
		Insert(poNode);
	}
}

template<class T>
void CRBTree<T>::Insert(CRBNode<T>* pNode)
{
	CRBNode<T>* pParent = nullptr;
	CRBNode<T>* pTemp = m_Root;

	//从根节点出发找插入点
	while (pTemp != nullptr)
	{
		pParent = pTemp;
		if (pTemp->m_tKey>pNode->m_tKey)
		{
			pTemp = pTemp->m_pLeft;
		}
		else if (pTemp->m_tKey<pNode->m_tKey)
		{
			pTemp = pTemp->m_pRight;
		}
		else
		{
			return;
		}
	}

	pNode->m_pParent = pParent;

	if (pParent != nullptr)
	{
		if (pParent->m_tKey>pNode->m_tKey)
		{
			pParent->m_pLeft = pNode;
		}
		else
		{
			pParent->m_pRight = pNode;
		}
	}
	else//如果插入点的父节点为空
	{
		m_Root = pNode;
	}

	InsertFixUp(pNode);
}

template<class T>
void CRBTree<T>::InsertFixUp(CRBNode<T>* pCur)
{
	CRBNode<T>* poParent = pCur->m_pParent;
	CRBNode<T>* poGParent;
	CRBNode<T>* poUncle;
	while (poParent != nullptr&&poParent->m_bColor == RED)
	{
		poGParent = poParent->m_pParent;//如果poParent为红色 则必定有父节点

										//如果父节点为祖节点的左节点
		if (poParent == poGParent->m_pLeft)
		{
			poUncle = poGParent->m_pRight;

			if (poUncle != nullptr&& poUncle->m_bColor == RED)
			{
				poParent->m_bColor = poUncle->m_bColor = BLACK;
				poGParent->m_bColor = RED;
				pCur = poGParent;
			}
			else
			{
				if (pCur == poParent->m_pRight)
				{
					LeftRotate(poParent);
					CRBNode<T>* poTmp = poParent;
					poParent = pCur;
					pCur = poTmp;
				}

				RightRotate(poGParent);
				poParent->m_bColor = BLACK;
				poGParent->m_bColor = RED;

				pCur = poParent;
			}
		}
		else
		{
			poUncle = poGParent->m_pLeft;

			if (poUncle != nullptr&&poUncle->m_bColor == RED)
			{
				poParent->m_bColor = poUncle->m_bColor = BLACK;
				poGParent->m_bColor = RED;
				pCur = poGParent;
			}
			else
			{
				if (pCur == poParent->m_pLeft)
				{
					RightRotate(poParent);
					CRBNode<T>* poTmp = poParent;
					poParent = pCur;
					pCur = poTmp;
				}

				LeftRotate(poGParent);
				poParent->m_bColor = BLACK;
				poGParent->m_bColor = RED;

				pCur = poGParent;
			}
		}
	}
	m_Root->m_bColor = BLACK;
}

