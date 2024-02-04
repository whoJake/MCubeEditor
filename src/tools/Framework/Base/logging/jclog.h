#pragma once

#include "Log.h"
#include "ConsoleTarget.h"
#include "FileTarget.h"
#include "EmptyTarget.h"

#define JCLOG_NONE(logref, fmt, ...) (logref).none(__FUNCTION__, fmt, __VA_ARGS__)
#define JCLOG_TRACE(logref, fmt, ...) (logref).trace(__FUNCTION__, fmt, __VA_ARGS__)
#define JCLOG_TRACK(logref, fmt, ...) (logref).track(__FUNCTION__, fmt, __VA_ARGS__)
#define JCLOG_DEBUG(logref, fmt, ...) (logref).debug(__FUNCTION__, fmt, __VA_ARGS__)
#define JCLOG_PROFILE(logref, fmt, ...) (logref).profile(__FUNCTION__, fmt, __VA_ARGS__)
#define JCLOG_INFO(logref, fmt, ...) (logref).info(__FUNCTION__, fmt, __VA_ARGS__)
#define JCLOG_WARN(logref, fmt, ...) (logref).warn(__FUNCTION__, fmt, __VA_ARGS__)
#define JCLOG_ERROR(logref, fmt, ...) (logref).error(__FUNCTION__, fmt, __VA_ARGS__)
#define JCLOG_EVENT(logref, fmt, ...) (logref).event(__FUNCTION__, fmt, __VA_ARGS__)
#define JCLOG_LOG(logref, level, fmt, ...) (logref).log(level, __FUNCTION__, fmt, __VA_ARGS__)