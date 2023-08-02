#include "gui.hpp"

#include "common.hpp"
#include "gui/constants.hpp"
#include "gui/utils.hpp"
#include "logging.hpp"

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#  include <wx/cmdline.h>
#  include <wx/wx.h>
#endif

// Make sure we have everything enabled in wx that we need
static_assert(wxUSE_MENUBAR && wxUSE_STATUSBAR, "Missing required wxWidgets features");

namespace krompir {
namespace gui {

// ----------------------------------------------------------------------------

// Define a new frame type: this is going to be our main frame
class MainFrame : public wxFrame {
    /**
     * Create the menu bar for this frame and set it.
     */
    void
    create_menu_bar_()
    {
        // Create our "File" menu
        auto* file_menu = new wxMenu();
        file_menu->Append(CONTROL_QUIT, "E&xit\tAlt-X", "Quit this program");

        // the "About" item should be in the help menu
        auto* help_menu = new wxMenu();
        help_menu->Append(CONTROL_ABOUT, "&About\tF1", "Show about dialog");

        // now append the freshly created menu to the menu bar...
        auto* menu_bar = new wxMenuBar();
        menu_bar->Append(file_menu, "&File");
        menu_bar->Append(help_menu, "&Help");

        // ... and attach this menu bar to the frame
        SetMenuBar(menu_bar);
    }

public:
    // ctor(s)
    explicit MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
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

private:
    /*
     * event handlers (these functions should _not_ be virtual or static)
     */

    /**
     * Called when the about button is pressed or an about command is sent to the
     * window.
     */
    void
    on_about_(wxCommandEvent& event)
    {
        UNUSED(event);

        wxMessageBox(
            wxString::Format(
                "Welcome to Krompir!\n"
                "\n"
                "Application Information:\n"
                " - %s\n"
                " - Binlog @ %s\n"
                " - %s\n",
                wxVERSION_STRING,
                BINLOG_VERSION,
                wxGetOsDescription()
            ),
            "About Krompir",
            wxOK | wxICON_INFORMATION, // NOLINT(hicpp-signed-bitwise)
            this
        );
    }
};

// ----------------------------------------------------------------------------

// Define the main application
class KrompirApp : public wxApp {
public:
    // override base class virtuals
    // ----------------------------

    /**
     * Called on startup, allows us to do initialization.
     *
     * If we return false, the app exists.
     */
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

        // Bind the idle event to allow us to process logs without blocking the user
        Bind(wxEVT_IDLE, [](wxIdleEvent&) { logging::process(); });

        // success: wxApp::OnRun() will be called which will enter the main message
        // loop and the application will run. If we returned false here, the
        // application would exit immediately.
        return true;
    }

    /**
     * Called on app exit, for us to do cleanup and return exit status.
     *
     * Right now, just want to process remaining log messages.
     */
    int
    OnExit() override
    {
        // Make sure everything got logged
        logging::process();

        return 0;
    }

    /**
     * Called before parsing command line.
     *
     * We parse command line arguments elsewhere, so clear the command line.
     */
    void
    OnInitCmdLine(wxCmdLineParser& parser) override
    {
        parser.SetCmdLine(0, static_cast<char**>(nullptr));
    }

    /**
     * Called when command line is parsed.
     *
     * We parse command line arguments elsewhere, so simply ignore
     */
    bool
    OnCmdLineParsed(wxCmdLineParser& parser) override
    {
        UNUSED(parser);

        // Do nothing
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
    log_i(gui, "Starting GUI!");
    log_d(gui, "wxWidgets v{}", get_wx_version_string());
    log_d(gui, "{}", wxGetOsDescription().utf8_string());

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
