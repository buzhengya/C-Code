#pragma once
#include "log.h"

#define ASSERT_RETVOID_MSG(condition, msg) \
	if (!(condition)) \
	{ \
		EXLOG_ERROR << "assert error : " << msg; \
		return; \
	}

#define ASSERT_RETVAL_MSG(condition, val, msg) \
	if (!(condition)) \
	{ \
		EXLOG_ERROR << "assert error : " << msg; \
		return val; \
	}

#define  ASSERT_RETVOID (condition, msg) \
	if (!(condition)) \
	{ \
		return; \
	}

#define ASSERT_RETVAL (condition, val) \
	if (!(condition))	\
	{ \
		return val; \
	}
