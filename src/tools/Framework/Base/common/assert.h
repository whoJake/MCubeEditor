#pragma once

#include "exceptions.h"

#ifdef CFG_DEBUG
#define ASSERT_WITH_MESSAGE(msg, ...) __debugbreak()
#else
#define ASSERT_WITH_MESSAGE(msg, ...) throw FormattedException(msg, __VA_ARGS__)
#endif

#define TRAP_NEQ(condition, msg, ...) if(!(condition)){ ASSERT_WITH_MESSAGE(msg, __VA_ARGS__); }
#define TRAP_EQ(condion, msg, ...) if(!!(condition)){ ASSERT_WITH_MESSAGE(msg, __VA_ARGS__); }
#define TRAP_NULLPTR(ptr, msg, ...) if((ptr) == nullptr){ ASSERT_WITH_MESSAGE(msg, __VA_ARGS__); }
