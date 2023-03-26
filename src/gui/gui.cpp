#include "gui.hpp"

#include "logging.hpp"

#include <GL/gl.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#if defined(IMGUI_IMPL_OPENGL_ES2)
#  include <GLES2/gl2.h>
#endif

#include <GLFW/glfw3.h> // Will drag system OpenGL headers

namespace krompir {

// NOLINTNEXTLINE(cert-err58-cpp)
static const auto LOG = spdlog::stderr_color_mt("gui");

namespace gui {

static void
glfw_error_callback(int error, const char* description)
{
    log_e("GLFW Error {}: {}", error, description);
}

static inline const char*
glGetString_p(GLenum id) // NOLINT(readability-identifier-*)
{
    const unsigned char* res = glGetString(id);

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    return reinterpret_cast<const char*>(res);
}

void
start()
{
    log_i("Starting GUI!");

    // Setup GLFW
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        log_c("Could not initialize OpenGL/GLFW");
        std::abort();
    }

    /*
     * Decide GL+GLSL versions
     */
#ifdef IMGUI_IMPL_OPENGL_ES2
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Krompir", NULL, NULL);
    if (!window) {
        log_c("Could not create OpenGL window");
        std::abort();
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Print OpenGL info
    log_d("OpenGL {}", glGetString_p(GL_VERSION));
    log_d("GLSL v{}", &glsl_version[9]); // skip the "#version" part
    log_d("Renderer: {}", glGetString_p(GL_RENDERER));
    log_d("Graphics Card: {}", glGetString_p(GL_VENDOR));

    // Print GLFW info
    GLint glfw_major;
    GLint glfw_minor;

    glGetIntegerv(GLFW_CONTEXT_VERSION_MAJOR, &glfw_major);
    glGetIntegerv(GLFW_CONTEXT_VERSION_MINOR, &glfw_minor);

    log_d("GLFW v{}.{}", glfw_major, glfw_minor);

    /*
     * IMGUI
     */
    log_d("IMGUI v{}", IMGUI_VERSION);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    // Setup Dear ImGui style
    // TODO make this selectable
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

// Load Fonts
#if 0
// - If no fonts are loaded, dear imgui will use the default font. You can also load
//   multiple fonts and use ImGui::PushFont()/PopFont() to select them.
// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to
//   select the font among multiple.
// - If the file cannot be loaded, the function will return NULL. Please handle
//   those errors in your application (e.g. use an assertion, or display an error
//   and quit).
// - The fonts will be rasterized at a given size (w/ oversampling) and stored into
//   a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which
//   ImGui_ImplXXXX_NewFrame below will call.
// - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for
//   higher quality font rendering.
// - Read 'docs/FONTS.md' for more instructions and details.
// - Remember that in C/C++ if you want to include a backslash \ in a string literal
//   you need to write a double backslash \\ !
// - Our Emscripten build process allows embedding fonts to be accessible at runtime
//   from the "fonts/" folder. See Makefile.emscripten for details.
    io.Fonts->AddFontDefault();
    io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    ImFont* font = io.Fonts->AddFontFromFileTTF(
        "c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL,
        io.Fonts->GetGlyphRangesJapanese()
    );
    IM_ASSERT(font != NULL);
#endif

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    do {
        /*
         * Poll and handle events (inputs, window resize, etc.)
         *
         * You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if
         * dear imgui wants to use your inputs.
         *
         * - When io.WantCaptureMouse is true, do not dispatch mouse input data to your
         *   main application, or clear/overwrite your copy of the mouse data.
         * - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to
         *   your main application, or clear/overwrite your copy of the keyboard data.
         *
         * Generally you may always pass all inputs to dear imgui, and hide them from
         * your application based on those two flags.
         */
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window
        // (Most of the sample code is in ImGui::ShowDemoWindow()!
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to
        // create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            // Create a window called "Hello, world!" and append into it.
            ImGui::Begin("Hello, world!");

            // Display some text (you can use a format strings too)
            ImGui::Text("This is some useful text.");

            // Edit bools storing our window open/close state
            ImGui::Checkbox("Demo Window", &show_demo_window);
            ImGui::Checkbox("Another Window", &show_another_window);

            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);

            // Edit 3 floats representing a color
            ImGui::ColorEdit3("clear color", reinterpret_cast<float*>(&clear_color));

            // Buttons return true when clicked
            // (most widgets return true when edited/activated)
            if (ImGui::Button("Button"))
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            auto framerate = static_cast<double>(io.Framerate);
            ImGui::Text(
                "Application average %.3f ms/frame (%.1f FPS)", 1000.0 / framerate,
                framerate
            );
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window) {
            ImGui::Begin(
                "Another Window", &show_another_window
            ); // Pass a pointer to our bool variable (the window will have a closing
               // button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        // Rendering
        ImGui::Render();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(
            clear_color.x * clear_color.w, clear_color.y * clear_color.w,
            clear_color.z * clear_color.w, clear_color.w
        );
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    } while (!glfwWindowShouldClose(window));
}

} // namespace gui

} // namespace krompir
