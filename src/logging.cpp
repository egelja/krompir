/**
 * @file logging.cpp
 * @brief Logging Utilities.
 * @copyright MIT
 *
 * Just some wrappers around SPDLOG to make it easier/configured to my liking.
 *
 */
#include "logging.hpp"

#include <spdlog/cfg/env.h>
#include <spdlog/common.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#define BACKTRACE_LENGTH 32

namespace krompir::logging {

/**
 * @brief Initialize logging for this project.
 */
void
init()
{
    // Change logger to stderr
    spdlog::set_default_logger(spdlog::stderr_color_mt("main"));

    // Set log level
    spdlog::set_level(spdlog::level::info);
    spdlog::cfg::load_env_levels();

    // Change logger pattern
    // [%Y-%m-%d %H:%M:%S.%e] [%n] [%l] [%s:%#] %v
    spdlog::set_pattern("%^[%Y-%m-%d %H:%M:%S.%e] [%10s:%-4#] [%=10n] - [%=8l] %v%$");

    // Enable backtrace buffer
    spdlog::enable_backtrace(BACKTRACE_LENGTH);

    // Send our first message
    log_i("Logging initialized successfully");
}

} // namespace krompir::logging
