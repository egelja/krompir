#include "logging.hpp"

#include <ios>

namespace {

/**
 * Convert a binlog severity to a color code.
 *
 * @param severity The severity.
 *
 * @returns A string containing an ANSI color code for that severity.
 */
const char*
severity_to_color(const binlog::Severity& severity)
{
    switch (severity) {
        case binlog::Severity::trace:
            return "\x1b[38;5;7m"; // Gray
        case binlog::Severity::debug:
            return "\x1b[36m"; // Cyan;
        case binlog::Severity::info:
            return "\x1b[32m"; // Green
        case binlog::Severity::warning:
            return "\x1b[1;33m"; // Bold Yellow
        case binlog::Severity::error:
            return "\x1b[1;31m"; // Bold red
        case binlog::Severity::critical:
            return "\x1b[1;37m\x1b[41m"; // Bold white on red
        case binlog::Severity::no_logs:
            return "\x1b[0m"; // Reset
        default:
            return "";
    }
}
} // namespace

namespace krompir {
namespace logging {

namespace detail {

ColoredTextOutputStream&
ColoredTextOutputStream::write(const char* data, std::streamsize size)
{
    const binlog::Range range{data, data + size}; // NOLINT(*-pointer-arithmetic)
    binlog::RangeEntryStream entry_stream(range);

    while (const binlog::Event* event = event_stream_.nextEvent(entry_stream)) {
        out_ << severity_to_color(event->source->severity);
        printer_.printEvent(
            out_, *event, event_stream_.writerProp(), event_stream_.clockSync()
        );
        out_ << "\x1b[0m"; // Reset
    }

    return *this;
}

MultiOutputStream&
MultiOutputStream::write(const char* data, std::streamsize size)
{
    binary_.write(data, size);

    try {
        filter_.writeAllowed(data, static_cast<size_t>(size), text_);
    } catch (const std::runtime_error& ex) {
        std::cerr << "Failed to convert buffer to text: " << ex.what() << "\n";
    }

    return *this;
}

} // namespace detail

} // namespace logging
} // namespace krompir
