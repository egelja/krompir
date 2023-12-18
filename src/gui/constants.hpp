#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#  include <wx/wx.h>
#endif

namespace krompir {
namespace gui {

// IDs for the controls and the menu commands
enum Control {
    // menu items
    CONTROL_QUIT = wxID_EXIT,

    // it is important for the id corresponding to the "About" command to have
    // this standard value as otherwise it won't be handled properly under Mac
    // (where it is special and put into the "Apple" menu)
    CONTROL_ABOUT = wxID_ABOUT
};

} // namespace gui
} // namespace krompir
