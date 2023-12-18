#pragma once

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#  include <wx/wx.h>
#endif

#include <fmt/format.h>

#include <string>

namespace krompir {
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

/**
 * Get the version of wx as a nice string.
 */
inline constexpr std::string
get_wx_version_string()
{
    return (                                 //
        wxSTRINGIZE(wxMAJOR_VERSION)         //
        "." wxSTRINGIZE(wxMINOR_VERSION)     //
        "." wxSTRINGIZE(wxRELEASE_NUMBER)    //
#ifdef wxSUBRELEASE_NUMBER                   //
        "." wxSTRINGIZE(wxSUBRELEASE_NUMBER) //
#endif                                       //
    );
}

} // namespace utils
} // namespace krompir
