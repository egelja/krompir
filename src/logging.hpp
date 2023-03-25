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
#undef SPDLOG_ACTIVE_LEVEL
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

// Give SPDLOG a better function on GCC/Clang
#if defined(__GNUC__) || defined(__clang__)
#  undef SPDLOG_FUNCTION
#  define SPDLOG_FUNCTION __PRETTY_FUNCTION__
#endif

// Now include the header
#include <spdlog/spdlog.h>

namespace krompir::logging {

/**
 * @brief Initialize logging for this project.
 */
void init();

} // namespace krompir::logging

// Wrap the SPDLOG macros in ones we like.
// NOLINTBEGIN
#define log_t(...) SPDLOG_TRACE(__VA_ARGS__) /* NOLINT */
#define log_d(...) SPDLOG_DEBUG(__VA_ARGS__) /* NOLINT */
#define log_i(...) SPDLOG_INFO(__VA_ARGS__)  /* NOLINT */
#define log_w(...) SPDLOG_WARN(__VA_ARGS__)  /* NOLINT */

// Want to dump backtrace for these two
#define log_e(...)                                                                     \
  do {                                                                                 \
    SPDLOG_ERROR(__VA_ARGS__); /* NOLINT */                                            \
    spdlog::dump_backtrace();                                                          \
  } while (0)

#define log_c(...)                                                                     \
  do {                                                                                 \
    SPDLOG_CRITICAL(__VA_ARGS__); /* NOLINT */                                         \
    spdlog::dump_backtrace();                                                          \
  } while (0)

// NOLINTEND
