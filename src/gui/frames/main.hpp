#pragma once

#include "common.hpp"
#include "gui/constants.hpp"

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#  include <wx/wx.h>
#endif

namespace gui {

/**
 * The main frame for this project.
 *
 * This is where the application will be spending most of its time.
 */
class MainFrame : public wxFrame {
    /**
     * Create the menu bar for this frame and set it.
     */
    void create_menu_bar_();

public:
    /**
     * Create a new main frame.
     *
     * Should really ever be called once.
     */
    explicit MainFrame(const wxString& title);

private:
    /*  event handlers (these functions should _not_ be virtual or static)   */

    /**
     * Called when the about button is pressed or an about command is sent to the
     * window.
     */
    void on_about_(wxCommandEvent& event);
};

} // namespace gui
