/**
 * @file logging.hpp
 * @brief Logging Utilities.
 * @copyright MIT
 *
 * Just some wrappers around SPDLOG to make it easier/configured to my liking.
 *
 * Some of this is adapted from binlog, licensed under Apache2.
 */
#pragma once

#include "config.h"

// Binlog itself
#include <binlog/binlog.hpp>
#include <binlog/Entries.hpp>
#include <binlog/EntryStream.hpp>
#include <binlog/EventStream.hpp>
#include <binlog/PrettyPrinter.hpp>

// Binlog internals
#include <binlog/EventFilter.hpp>
#include <binlog/Session.hpp>
#include <binlog/SessionWriter.hpp>
#include <binlog/Severity.hpp>
#include <binlog/TextOutputStream.hpp> // requires binlog library to be linked

// Binlog extensions
#include <binlog/adapt_stdduration.hpp>
#include <binlog/adapt_stderrorcode.hpp>
#include <binlog/adapt_stdfilesystem.hpp>
#include <binlog/adapt_stdoptional.hpp>
#include <binlog/adapt_stdtimepoint.hpp>
#include <binlog/adapt_stdvariant.hpp>
#include <binlog/default_session.hpp>

#include <fstream>
#include <ios>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <thread>

namespace krompir {
namespace logging {

namespace detail {

#if DEBUG()
static constexpr auto LOG_CONSOLE_SEVERITY = binlog::Severity::debug;
#else
static constexpr auto LOG_CONSOLE_SEVERITY = binlog::Severity::error;
#endif

/**
 * Convert a binlog stream to text, and color it.
 *
 * Adapted from binlog::TextOutputStream, licensed under Apache2.
 */
class ColoredTextOutputStream {
    std::ostream& out_;
    binlog::EventStream event_stream_;
    binlog::PrettyPrinter printer_;

public:
    /**
     * Create a new ColoredTextOutputStream.
     */
    explicit ColoredTextOutputStream(
        std::ostream& out,
        std::string event_format = "%S %C [%d] %n %m (%G:%L)\n",
        std::string date_format = "%Y-%m-%d %H:%M:%S.%N"
    ) :
        out_(out), printer_(std::move(event_format), std::move(date_format))
    {}

    /**
     * Write data to the stream.
     */
    ColoredTextOutputStream& write(const char* data, std::streamsize size);
};

/**
 * Write complete binlog output to `binary`,
 * and also write error and above events to `text` - as text.
 *
 * https://binlog.org/UserGuide.html#multiple-output
 */
class MultiOutputStream {
    std::ostream& binary_;
    ColoredTextOutputStream text_;
    binlog::EventFilter filter_;

public:
    /**
     * Create a new MultiOutputStream.
     */
    MultiOutputStream(std::ostream& binary, std::ostream& text) :
        binary_(binary),
        text_(text, "%S %C [%d] %n %m (%G:%L)\n"),
        filter_([](const binlog::EventSource& source) {
            return source.severity >= LOG_CONSOLE_SEVERITY;
        })
    {}

    /**
     * Write data to the stream.
     */
    MultiOutputStream& write(const char* data, std::streamsize size);
};

} // namespace detail

/**
 * Convert std::this_thread::get_id() to string
 *
 * Taken from binlog, licensed under Apache2.
 */
inline std::string
this_thread_id_string()
{
    std::ostringstream str;
    str << std::this_thread::get_id();
    return str.str();
}

/**
 * Get a thread-local writer for logging.
 *
 * This writer is used by basic log macros.
 *
 * The implementation uses a function local static,
 * avoid using the returned reference in the context
 * of global destructors.
 *
 * Taken from binlog, licensed under Apache2.
 */
inline binlog::SessionWriter&
thread_local_writer()
{
    static thread_local binlog::SessionWriter writer(
        binlog::default_session(),
        KROMPIR_LOG_QUEUE_SIZE, // queue capacity
        0,                      // writer id
        this_thread_id_string() // writer name
    );
    return writer;
}

inline void
process()
{
    // TODO(egelja): put the log file in an intelligent location
    static std::ofstream log_file(
        "krompir.blog", std::ofstream::out | std::ofstream::binary
    );
    static detail::MultiOutputStream output(log_file, std::cerr);

    binlog::consume(output);
}

} // namespace logging
} // namespace krompir

// Logging Macros
// Adapted from binlog, licensed under Apache2
// NOLINTBEGIN
#define log_t(category, ...)                                                           \
    BINLOG_TRACE_WC(krompir::logging::thread_local_writer(), category, __VA_ARGS__)

#define log_d(category, ...)                                                           \
    BINLOG_DEBUG_WC(krompir::logging::thread_local_writer(), category, __VA_ARGS__)

#define log_i(category, ...)                                                           \
    BINLOG_INFO_WC(krompir::logging::thread_local_writer(), category, __VA_ARGS__)

#define log_w(category, ...)                                                           \
    BINLOG_WARN_WC(krompir::logging::thread_local_writer(), category, __VA_ARGS__)

#define log_e(category, ...)                                                           \
    BINLOG_ERROR_WC(krompir::logging::thread_local_writer(), category, __VA_ARGS__)

#define log_c(category, ...)                                                           \
    BINLOG_CRITICAL_WC(krompir::logging::thread_local_writer(), category, __VA_ARGS__)
// NOLINTEND
