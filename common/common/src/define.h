#pragma once
#include <iostream>
#include <assert.h>

typedef int						int32;
typedef unsigned int			uint32;
typedef long long				int64;
typedef unsigned long long		uint64;

#define ASSERT  assert
#define INVALID_32BIT ~0

#define ALIGNMENT sizeof(unsigned long)

#define wind_malloc(size)	malloc(size)
#define wind_free(ptr)	free(ptr)
#define wind_align(p, n) (char *)((uint64(p) + uint64(n) - 1) & ~(uint64(n) - 1))
