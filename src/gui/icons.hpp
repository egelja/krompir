#pragma once

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#  include <wx/wx.h>
#endif

#include <optional>

namespace krompir {
namespace gui {
namespace icons {

/**
 * Get an icon by size
 *
 * @param size The size of the icon.
 *
 * @returns A wxIcon representing that icon, if it exists.
 */
std::optional<wxIcon> get_icon(size_t size);

/**
 * Get an icon XPM.
 *
 * @param size The size of the icon.
 *
 * @returns The XPM as an array of `const char*`, or nullptr.
 */
const char** get_xpm(size_t size);

/**
 * Get all bundled icons.
 *
 * @returns An icon bundle containing all the application icons.
 */
wxIconBundle get_all_icons();

} // namespace icons
} // namespace gui
} // namespace krompir
