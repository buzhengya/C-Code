#pragma once
#include <iostream>
#include <assert.h>

typedef int						int32;
typedef unsigned int			uint32;
typedef long long				int64;
typedef unsigned long long		uint64;

#define LOG		std::cout
#define ASSERT  assert

#define wind_malloc(size)	malloc(size)
#define wind_free(ptr)	free(ptr)
