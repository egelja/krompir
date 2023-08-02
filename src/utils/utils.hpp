#pragma once

#include "logging.hpp"

#include <fmt/core.h>
#include <fmt/xchar.h>

#include <string>
#include <typeinfo>

/**
 * @brief A safe version of new.
 *
 * Guaranteed to never return nullptr.
 */
template <class T, class... Args>
inline T*
new_s(Args... args)
{
    auto* ptr = new T(std::forward<Args>(args)...);
    if (ptr == nullptr) {
        log_e(main, "Could not allocate a new {}", typeid(T).name());
        log_c(main, "Out of memory!");

        abort();
    }

    return ptr;
}

namespace utils {

/**
 * @brief Get the version of {fmt} as a nice string.
 */
inline std::string
fmt_version_str()
{
    uint8_t major = FMT_VERSION / 10000;       // NOLINT(*-magic-numbers)
    uint8_t minor = (FMT_VERSION / 100) % 100; // NOLINT(*-magic-numbers)
    uint8_t patch = FMT_VERSION % 100;         // NOLINT(*-magic-numbers)

    return fmt::format("{}.{}.{}", major, minor, patch);
}

} // namespace utils
