#pragma once
#include "log.h"

#include <assert.h>

#define ASSERT_RETVOID(condition) if (!(condition)) \
{\
	LOG_ERROR << #condition << " error. function : " << __FUNCTION__;\
	assert(condition);\
	return;\
}\

#define ASSERT_RETVOID_MSG(condition, msg) if (!(condition)) \
{\
	LOG_ERROR << #condition << " error. function : " << __FUNCTION__ << " msg : " << msg;\
	assert(condition);\
	return;\
}\


#define ASSERT_RETVAL(condition, val) if (!(condition)) \
{\
	LOG_ERROR << #condition << " error. function : " << __FUNCTION__;\
	assert(condition);\
	return val;\
}\


#define ASSERT_RETVAL_MSG(condition,val, msg) if (!(condition)) \
{\
	LOG_ERROR << #condition << " error. function : " << __FUNCTION__ << " msg : " << msg;\
	assert(condition);\
	return val;\
}\


#define ASSERT_CONTINUE(condition) if (!(condition)) \
{\
	LOG_ERROR << #condition << " error. function : " << __FUNCTION__;\
	assert(condition);\
	continue;\
}\


#define ASSERT_CONTINUE_MSG(condition, msg) if (!(condition)) \
{\
	LOG_ERROR << #condition << " error. function : " << __FUNCTION__ << " msg : " << msg;\
	assert(condition);\
	continue;\
}\

