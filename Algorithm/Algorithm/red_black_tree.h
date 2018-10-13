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
void CRBTree<T>::LeftRotate(CRBNode<T>* pCur)//���� pCur��ʾ��ǰ�ڵ� pRight��ʾpCur�����ӽڵ� pParent��ʾpCur�ĸ��ڵ�
{
	CRBNode<T>* pRight = pCur->m_pRight;
	CRBNode<T>* pParent = pCur->m_pParent;

	//�Ե�ǰ�ڵ�Ĳ���
	pCur->m_pRight = pRight->m_pLeft;
	pCur->m_pParent = pRight;

	//���ӽڵ�Ĳ���
	pRight->m_pLeft = pCur;
	pRight->m_pParent = pParent;

	//�Ը��ڵ����
	if (pParent == nullptr)//Ϊ������ĸ��ڵ�
	{
		m_Root = pRight;
	}
	else
	{
		if (pCur == pParent->m_pLeft)//���ԭ�ڵ��Ǹ��ڵ�����ӽڵ�
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
void CRBTree<T>::RightRotate(CRBNode<T>* pCur)//���� x��ʾ��ǰ�ڵ� y��ʾx�����ӽڵ� p��ʾx�ĸ��ڵ�
{
	CRBNode<T>* pLeft = pCur->m_pLeft;
	CRBNode<T>* pParent = pCur->m_pParent;

	//�Ե�ǰ�ڵ�Ĳ���
	pCur->m_pLeft = pLeft->m_pRight;
	pCur->m_pParent = pLeft;

	//���ӽڵ�Ĳ���
	pLeft->m_pRight = pCur;
	pLeft->m_pParent = pParent;

	//�Ը��ڵ����
	if (pParent == nullptr)//Ϊ������ĸ��ڵ�
	{
		m_Root = pLeft;
	}
	else
	{
		if (pCur == pParent->m_pLeft)//���ԭ�ڵ��Ǹ��ڵ�����ӽڵ�
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

	//�Ӹ��ڵ�����Ҳ����
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
	else//��������ĸ��ڵ�Ϊ��
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
		poGParent = poParent->m_pParent;//���poParentΪ��ɫ ��ض��и��ڵ�

										//������ڵ�Ϊ��ڵ����ڵ�
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

