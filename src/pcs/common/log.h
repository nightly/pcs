#pragma once

#if defined _DEBUG == 1
	#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#else 
	#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_OFF
#endif 

// Log level must be defined prior to including spdlog.h
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/fmt/ranges.h>
#include <spdlog/fmt/bundled/color.h>

#if defined _DEBUG == 1
	#define PCS_TRACE(...) SPDLOG_TRACE( __VA_ARGS__)
	#define PCS_DEBUG(...) SPDLOG_DEBUG( __VA_ARGS__)
	#define PCS_INFO(...) SPDLOG_INFO( __VA_ARGS__)
	#define PCS_WARN(...) SPDLOG_WARN( __VA_ARGS__)
	#define PCS_ERROR(...) SPDLOG_ERROR( __VA_ARGS__)
	#define PCS_CRITICAL(...) SPDLOG_CRITICAL( __VA_ARGS__)
#else 
	#define PCS_INFO(...)
	#define PCS_DEBUG(...)
	#define PCS_TRACE(...)
	#define PCS_WARN(...)
	#define PCS_ERROR(...)
	#define PCS_CRITICAL(...)
#endif