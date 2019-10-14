#include "event_timer.h"
#include "../common/time.h"
#include "../rbtree/rb_tree.h"

static RBTree* pTimerTree;
static RBTreeNode* pTimerLeaf;

// Q1 : repeat timer?  Q2 : timer precision?
// A2 : 
bool AddTimer(Event *ev, int64 nDelay)
{
	if (nDelay < 0) nDelay = 0;
	int64 nExpire = nDelay + GetNowMilSec();
	ev->oNode.nKey = nExpire;
	return RBTreeInsert(pTimerTree, &ev->oNode);
}

bool DelTimer(Event * ev)
{
	return RBTreeDelete(pTimerTree, &ev->oNode);
}

bool InitEventTimer()
{
	pTimerTree = RBTreeInit();
	pTimerLeaf = pTimerTree->pLeaf;
	ASSERT(pTimerTree != nullptr);
	return true;
}

int64 FindExpireTimer()
{
	// 3 case.  case 1 : no expire  case 2  has expire  case 3 tree is empty.
	if (pTimerTree->pRoot == pTimerLeaf)
	{
		return EVENT_TIMER_INFINITE;
	}

	int64 nNow = GetNowMilSec();
	RBTreeNode * pMinNode = MinNode(pTimerTree->pRoot, pTimerLeaf);
	int64 nDiff = pMinNode->nKey - nNow;
	return nDiff > 0 ? nDiff : 0;
}

bool DealExpireTimer()
{
	RBTreeNode* pRoot = nullptr;
	RBTreeNode* pMin = nullptr;
	Event* ev = nullptr;
	int64 nNow = GetNowMilSec();
	while (true)
	{
		pRoot = pTimerTree->pRoot;
		if (pRoot == pTimerLeaf) break;
		
		pMin = MinNode(pRoot, pTimerLeaf);
		ev = (Event*)((char*)pMin - offsetof(Event, oNode));
		ev->fHandle(ev);
		ASSERT(DelTimer(ev) == true);
	}
	return true;
}

bool IsNoLeftTimer()
{
	if (pTimerTree->pRoot == pTimerLeaf)
	{
		return true;
	}

	int64 nNow = GetNowMilSec();
	RBTreeNode* pNode = MinNode(pTimerTree->pRoot, pTimerLeaf);
	Event* ev;
	while (pNode != nullptr)
	{
		ev = (Event*)((char *)pNode -offsetof(Event, oNode));
		if (ev->nCancle != 1)
		{
			return false;
		}
		pNode = NextNode(pNode, pTimerLeaf);
	}
	return true;
}