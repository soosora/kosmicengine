#pragma once

#ifndef FMT_HEADER_ONLY
#define FMT_HEADER_ONLY
#endif

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

// Initialize the Kosmic logger
namespace Kosmic {
namespace Log {
    inline void Init() {
        spdlog::set_pattern("%^[%T] [KOSMIC - %l]: %v%$");
        spdlog::set_level(spdlog::level::trace);
    }
} // namespace Log
} // namespace Kosmic

// Kosmic logging macros
#define KOSMIC_ERROR(...)  spdlog::error(__VA_ARGS__)
#define KOSMIC_INFO(...)   spdlog::info(__VA_ARGS__)
#define KOSMIC_WARN(...)   spdlog::warn(__VA_ARGS__)
#define KOSMIC_TRACE(...)  spdlog::trace(__VA_ARGS__)
