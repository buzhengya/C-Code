#pragma once
#include<time.h>
#define MAX_LEVEL 32

struct SkipListLevel
{
	struct SkipListNode* pForward;
	unsigned int span;
};
struct SkipListNode
{
	int nValue;
	double dScore;
	struct SkipListNode* pBackword;
	SkipListLevel Level[];
};

struct SkipList
{
	struct SkipListNode *pHead, *pTail;
	unsigned long lLength;
	int nLevel;
};

SkipListNode* NewSkipListNode(int nLevel,int nValue,double dScore)
{
	SkipListNode* pSkipListNode = (SkipListNode*)malloc(sizeof(SkipListNode) + nLevel * sizeof(SkipListLevel));
	pSkipListNode->nValue = nValue;
	pSkipListNode->dScore = dScore;
	pSkipListNode->pBackword = nullptr;
	return pSkipListNode;
}

SkipList* NewSkipList()
{
	SkipList* pSkipList = new SkipList;
	
	//初始化
	pSkipList->nLevel = 0;
	pSkipList->lLength = 0;
	pSkipList->pTail = nullptr;
	
	//初始化head
	pSkipList->pHead = NewSkipListNode(MAX_LEVEL, 0, 0);
	for(int i=0;i<MAX_LEVEL;i++)
	{
		pSkipList->pHead->Level[i].pForward = nullptr;
		pSkipList->pHead->Level[i].span = 0;
	}

	return pSkipList;
}

bool Insert(SkipList* pSkipList,int nValue,double dScore)
{
	SkipListNode* update[MAX_LEVEL], *x;
	int rank[MAX_LEVEL];
	int i = 0, nLevel;

	//更新update数组和rank数组
	x = pSkipList->pHead;
	for(i=pSkipList->nLevel-1;i>=0;i--)
	{
		rank[i] = i == pSkipList->nLevel - 1 ? 0 : rank[i + 1];
		while (x->Level[i].pForward != nullptr&&x->Level[i].pForward->dScore <= dScore)
		{
			if(x->Level[i].pForward->dScore==dScore&&x->Level[i].pForward->nValue==nValue)
			{
				return false;
			}
			rank[i] += x->Level[i].span;
			x = x->Level[i].pForward;
		}
		update[i] = x;
	}

	//
	srand(time(nullptr));
	nLevel = rand() % MAX_LEVEL + 1;
	x = NewSkipListNode(nLevel, nValue, dScore);

	if(nLevel> pSkipList->nLevel)
	{
		for (i = nLevel - 1; i >= pSkipList->nLevel; i--)
		{
			update[i] = pSkipList->pHead;
			update[i]->Level[i].pForward = nullptr;
			update[i]->Level[i].span = pSkipList->lLength;
			rank[i] = 0;
		}
	}

	//更新各节点的Level
	for (i = nLevel - 1; i >= 0; i--)
	{
		x->Level[i].pForward = update[i]->Level[i].pForward;
		x->Level[i].span = update[i]->Level[i].span - rank[0];

		update[i]->Level[i].pForward = x;
		update[i]->Level[i].span = rank[0] - rank[i] + 1;
	}

	if(nLevel<pSkipList->nLevel)
	{
		for (i = pSkipList->nLevel - 1; i >= nLevel; i--)
		{
			update[i]->Level[i].span++;
		}
	}

	//更新nLevel和Length
	pSkipList->lLength++;
	pSkipList->nLevel = pSkipList->nLevel > nLevel ? pSkipList->nLevel : nLevel;

	//更新pBackword
	if(update[0]==pSkipList->pHead)
	{
		x->pBackword = nullptr;
	}
	else
	{
		x->pBackword = update[0];
	}
	if(x->Level[0].pForward!=nullptr)
	{
		x->Level[0].pForward->pBackword = x;
	}

	//更新pTail
	if(x->Level[0].pForward==nullptr)
	{
		pSkipList->pTail = x;
	}
	return true;
}

void PrintSkipListNode(SkipListNode* pSkipListNode)
{
	printf("Value :%-10d  Score :%13.3f\n",pSkipListNode->nValue,pSkipListNode->dScore);
}

void PrintSkipList(SkipList* pSkipList)
{
	SkipListNode* pSkipListNode = pSkipList->pTail;
	while(pSkipListNode!=nullptr)
	{
		PrintSkipListNode(pSkipListNode);
		pSkipListNode = pSkipListNode->pBackword;
	}
}
