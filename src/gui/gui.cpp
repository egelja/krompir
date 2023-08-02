#include "gui.hpp"

#include "common.hpp"
#include "frames/frames.hpp"
#include "logging.hpp"
#include "utils.hpp"

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#  include <wx/cmdline.h>
#  include <wx/wx.h>
#endif

namespace krompir {
namespace gui {

/**
 * @brief The application class, which defines the entire wxWidgets application.
 */
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
