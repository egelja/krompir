#include "gui.hpp"

#include "gui/constants.hpp"
#include "gui/utils.hpp"
#include "logging.hpp"

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#  include <wx/wx.h>
#endif

// Make sure we have everything enabled in wx that we need
static_assert(wxUSE_MENUBAR && wxUSE_STATUSBAR, "Missing required wxWidgets features");

// Grab our logger
// NOLINTNEXTLINE(cert-err58-cpp)
static const auto LOG = spdlog::stderr_color_mt("gui");

namespace krompir {
namespace gui {

// ----------------------------------------------------------------------------

// Define a new frame type: this is going to be our main frame
class MainFrame : public wxFrame {
public:
    // ctor(s)
    explicit MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
    {
        // set the frame icon
        SetIcon(wxICON(sample));

        // create a menu bar
        auto* file_menu = new wxMenu();
        file_menu->Append(Control::QUIT, "E&xit\tAlt-X", "Quit this program");

        // the "About" item should be in the help menu
        auto* help_menu = new wxMenu();
        help_menu->Append(Control::ABOUT, "&About\tF1", "Show about dialog");

        // now append the freshly created menu to the menu bar...
        auto* menu_bar = new wxMenuBar();
        menu_bar->Append(file_menu, "&File");
        menu_bar->Append(help_menu, "&Help");

        // ... and attach this menu bar to the frame
        SetMenuBar(menu_bar);

        // create a status bar just for fun (by default with 1 pane only)
        CreateStatusBar(2);
        SetStatusText("Welcome to Krompir!");
    }

    // event handlers (these functions should _not_ be virtual)
    void
    on_quit(wxCommandEvent& WXUNUSED(event))
    {
        // true is to force the frame to close
        Close(true);
    }

    void
    on_about(wxCommandEvent& WXUNUSED(event))
    {
        wxMessageBox(
            wxString::Format(
                "Welcome to Krompir!\n"
                "\n"
                "Application Information:\n"
                " - %s\n"
                " - %s\n",
                wxVERSION_STRING, wxGetOsDescription()
            ),
            "About Krompir",
            wxOK | wxICON_INFORMATION, // NOLINT(hicpp-signed-bitwise)
            this
        );
    }

private:
    // any class wishing to process wxWidgets events must use this macro
    wxDECLARE_EVENT_TABLE(); // NOLINT
};

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
wxBEGIN_EVENT_TABLE(MainFrame, wxFrame) // NOLINT
/**/ EVT_MENU(Control::QUIT, MainFrame::on_quit)
/**/ EVT_MENU(Control::ABOUT, MainFrame::on_about)
wxEND_EVENT_TABLE()

// ----------------------------------------------------------------------------

// Define the main application
class KrompirApp : public wxApp {
public:
    // override base class virtuals
    // ----------------------------

    // this one is called on application startup and is a good place for the app
    // initialization (doing it here and not in the ctor allows to have an error
    // return: if OnInit() returns false, the application terminates)
    bool
    OnInit() override
    {
        // call the base class initialization method, currently it only parses a
        // few common command-line options but it could be do more in the future
        if (!wxApp::OnInit())
            return false;

        // create the main application window
        auto* frame = new MainFrame("Krompir");

        // and show it (the frames, unlike simple controls, are not shown when
        // created initially)
        frame->Show(true);

        // success: wxApp::OnRun() will be called which will enter the main message
        // loop and the application will run. If we returned false here, the
        // application would exit immediately.
        return true;
    }
};

// ----------------------------------------------------------------------------

// Implement the functions needed to run the app using wxWidgets
wxIMPLEMENT_APP_NO_MAIN(KrompirApp); // NOLINT

// Our "main" function for GUIs
int
main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    // Log some info about libraries and OS
    log_i("Starting GUI!");
    log_d("wxWidgets v{}", get_wx_version_string());
    log_d("{}", wxGetOsDescription().utf8_string());

    // Turn off debug support in release builds
    wxDISABLE_DEBUG_SUPPORT();

    // Run the app
    // On Windows WX does not trust the command line arguments and gets them in a
    // different way
    // However on non-windows it requires the arguments to be provided.
#ifdef __WINDOWS__
    return wxEntry();
#else
    return wxEntry(argc, argv);
#endif
}

} // namespace gui

} // namespace krompir
