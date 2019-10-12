#pragma once
#include "../rbtree/rb_tree.h"

struct Event
{
	int64		nExpire;
	RBTreeNode	oNode;
};