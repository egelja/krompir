#include "icons.hpp"

namespace {

// Include XPMs
#include "xpm/krompir_128.xpm"
#include "xpm/krompir_16.xpm"
#include "xpm/krompir_256.xpm"
#include "xpm/krompir_32.xpm"
#include "xpm/krompir_64.xpm"
#include "xpm/krompir_8.xpm"

} // namespace

namespace krompir {
namespace gui {
namespace icons {

std::optional<wxIcon>
get_icon(size_t size)
{
    // NOLINTBEGIN(*-magic-numbers)
    switch (size) {
        case 8:
            return wxICON(krompir_8);
        case 16:
            return wxICON(krompir_16);
        case 32:
            return wxICON(krompir_32);
        case 64:
            return wxICON(krompir_64);
        case 128:
            return wxICON(krompir_128);
        case 256:
            return wxICON(krompir_256);
        default:
            return {};
    }
    // NOLINTEND(*-magic-numbers)
}

const char**
get_icon_xpm(size_t size)
{
    // NOLINTBEGIN(*-magic-numbers)
    switch (size) {
        case 8:
            return krompir_8_xpm;
        case 16:
            return krompir_16_xpm;
        case 32:
            return krompir_32_xpm;
        case 64:
            return krompir_64_xpm;
        case 128:
            return krompir_128_xpm;
        case 256:
            return krompir_256_xpm;
        default:
            return nullptr;
    }
    // NOLINTEND(*-magic-numbers)
}

wxIconBundle
get_all_icons()
{
    wxIconBundle bundle;

    // Add all icons to bundle
    bundle.AddIcon(wxICON(krompir_8));
    bundle.AddIcon(wxICON(krompir_16));
    bundle.AddIcon(wxICON(krompir_32));
    bundle.AddIcon(wxICON(krompir_64));
    bundle.AddIcon(wxICON(krompir_128));
    bundle.AddIcon(wxICON(krompir_256));

    return bundle;
}

} // namespace icons
} // namespace gui
} // namespace krompir
