#pragma once
#include "../rbtree/rb_tree.h"

struct Event;

typedef void (*EventHandlePt)(Event* ev);

struct Event
{
	RBTreeNode	oNode;
	uint32		nCancle : 1;
	EventHandlePt	fHandle;
};
