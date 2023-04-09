/**
 * @file logging.hpp
 * @brief Logging Utilities.
 * @copyright MIT
 *
 * Just some wrappers around SPDLOG to make it easier/configured to my liking.
 *
 */
#pragma once

// We'll do any log level selecting at runtime
#include <string>
#undef SPDLOG_ACTIVE_LEVEL
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

// Give SPDLOG a better function on GCC/Clang
#if defined(__GNUC__) || defined(__clang__)
#  undef SPDLOG_FUNCTION
#  define SPDLOG_FUNCTION __PRETTY_FUNCTION__
#endif

// Now include the headers
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace krompir {

namespace logging {

/**
 * @brief Initialize logging for this project.
 */
std::shared_ptr<spdlog::logger> init();

inline auto
get_logger(const std::string& name) noexcept
{
    if (auto logger = spdlog::get(name))
        return logger;

    return spdlog::stderr_color_mt(name);
}

} // namespace logging

} // namespace krompir

// Wrap the SPDLOG macros in ones we like.
// NOLINTBEGIN
#define log_t(...) SPDLOG_LOGGER_TRACE(LOG, __VA_ARGS__)
#define log_d(...) SPDLOG_LOGGER_DEBUG(LOG, __VA_ARGS__)
#define log_i(...) SPDLOG_LOGGER_INFO(LOG, __VA_ARGS__)
#define log_w(...) SPDLOG_LOGGER_WARN(LOG, __VA_ARGS__)

// Want to dump backtrace for these two
#define log_e(...)                                                                     \
  do {                                                                                 \
    SPDLOG_LOGGER_ERROR(LOG, __VA_ARGS__);                                             \
    spdlog::dump_backtrace();                                                          \
  } while (0)

#define log_c(...)                                                                     \
  do {                                                                                 \
    SPDLOG_LOGGER_CRITICAL(LOG, __VA_ARGS__);                                          \
    spdlog::dump_backtrace();                                                          \
  } while (0)

// NOLINTEND
