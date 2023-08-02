#include "main.hpp"

#include "utils/utils.hpp"

#include <fmt/core.h>

namespace gui {

void
MainFrame::create_menu_bar_()
{
    // Create our "File" menu
    auto* file_menu = new_s<wxMenu>();
    file_menu->Append(CONTROL_QUIT, "E&xit\tAlt-X", "Quit this program");

    // the "About" item should be in the help menu
    auto* help_menu = new_s<wxMenu>();
    help_menu->Append(CONTROL_ABOUT, "&About\tF1", "Show about dialog");

    // now append the freshly created menu to the menu bar...
    auto* menu_bar = new_s<wxMenuBar>();
    menu_bar->Append(file_menu, "&File");
    menu_bar->Append(help_menu, "&Help");

    // ... and attach this menu bar to the frame
    SetMenuBar(menu_bar);
}

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
{
    // set the frame icon
    SetIcon(wxICON(sample));

    // create a menu bar
    create_menu_bar_();

    // create a status bar just for fun (by default with 1 pane only)
    CreateStatusBar(2);
    SetStatusText("Welcome to Krompir!");

    // And bind events
    Bind(
        wxEVT_MENU, [this](wxCommandEvent&) { Close(true); }, CONTROL_QUIT
    );
    Bind(wxEVT_MENU, &MainFrame::on_about_, this, CONTROL_ABOUT);
}

/*****************************************************************************
 *    event handlers (these functions should _not_ be virtual or static)     *
 *****************************************************************************/

void
MainFrame::on_about_(wxCommandEvent& event)
{
    UNUSED(event);

    wxMessageBox(
        wxString::Format(
            "Welcome to Krompir!\n"
            "\n"
            "Application Information:\n"
            " - %s\n"
            " - {fmt} v%s\n"
            " - Binlog @ %s\n"
            " - %s\n",
            wxVERSION_STRING,
            ::utils::fmt_version_str().c_str(),
            BINLOG_VERSION,
            wxGetOsDescription()
        ),
        "About Krompir",
        wxOK | wxICON_INFORMATION, // NOLINT(hicpp-signed-bitwise)
        this
    );
}

} // namespace gui