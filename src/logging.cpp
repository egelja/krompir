/**
 * @file logging.cpp
 * @brief Logging Utilities.
 * @copyright MIT
 *
 * Just some wrappers around SPDLOG to make it easier/configured to my liking.
 *
 */
#include "logging.hpp"

#include "config.h"

#include <spdlog/cfg/env.h>
#include <spdlog/common.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace krompir::logging {

/**
 * @brief Initialize logging for this project.
 */
std::shared_ptr<spdlog::logger>
init()
{
    // Change logger to stderr
    spdlog::set_default_logger(spdlog::stderr_color_mt("main"));

    // Set log level
#if DEBUG()
    spdlog::set_level(spdlog::level::debug);
#else
    spdlog::set_level(spdlog::level::info);
#endif

    // Check if log level changed in environment
    spdlog::cfg::load_env_levels();

    // Change logger pattern
    // [%Y-%m-%d %H:%M:%S.%e] [%n] [%l] [%s:%#] %v
    spdlog::set_pattern("%^[%Y-%m-%d %H:%M:%S.%e] [%25s:%-4#] [%=8l] - [%n] %v%$");

    // Enable backtrace buffer
    spdlog::enable_backtrace(BACKTRACE_LENGTH);

    // Send our first message
    SPDLOG_INFO("Logging initialized successfully");

    // Return root logger
    return spdlog::get("main");
}

} // namespace krompir::logging
