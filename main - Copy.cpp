#define USE_GLAD

#include "platform.hpp"
//#include "GUIShaderEdit.hpp"
#include "GUILauncher.hpp"
#include "staticgeom.hpp"

//#undef SDL_USE_BUILTIN_OPENGL_DEFINITIONS

/*
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif
*/

#include<iostream>
struct appscope {
    appscope() {
        SDL_Log("%s\n",__FUNCTION__);
        printf("writeme\n");
        std::cout<<"reality\n";
    }
    ~appscope() {
        SDL_Log("%s\n", __FUNCTION__);
    }
};

// Main code
//int main(int, char**)
int main(int argc, char* argv[])
{
    appscope mso;
    //~appscope::~appscope;
    //appscope::~mso();
    delete &mso;
    // Setup SDL
    // (Some versions of SDL before <2.0.10 appears to have performance/stalling issues on a minority of Windows systems,
    // depending on whether SDL_INIT_GAMECONTROLLER is enabled or disabled.. updating to latest version of SDL is recommended!)
    //SDL_SetMainReady();
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 300 es";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

// Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("Dear ImGui SDL2+OpenGL33333333333333333 example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

#ifdef USE_GLAD
    gladLoadGLES2Loader(SDL_GL_GetProcAddress);
#endif

    SDL_Log("Vendor   : %s", glGetString(GL_VENDOR));
    SDL_Log("Renderer : %s", glGetString(GL_RENDERER));
    SDL_Log("Version  : %s", glGetString(GL_VERSION));
    SDL_Log("GLSL     : %s", glGetString(GL_SHADING_LANGUAGE_VERSION));


    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Our state
    bool show_demo_window = false;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    bool show_demo_dlg = false;
    static bool shaderedit = false;
    std::string lastview = "";

    bool showguilauncher = true;

    //create default/UI-indepdent resources (default mesh, shaders, etc)
    CLEARGLERR();
    GLuint tprogram = 0;
    genLitGeosphere(0, 0);

    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();

        ImGui::NewFrame();

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                ImGui::MenuItem("Menu item");
                if (ImGui::BeginMenu("More.."))
                {
                    ImGui::MenuItem("Hello");
                    ImGui::MenuItem("Sailor");
                    if (ImGui::BeginMenu("Recurse.."))
                    {
                        //ShowExampleMenuFile();
                        ImGui::MenuItem("Nested Menu item");
                        ImGui::EndMenu();
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Views")) {
                ImGui::MenuItem("Single Image");
                ImGui::MenuItem("Texture Browser");
                ImGui::MenuItem("Shader edit", NULL, &shaderedit);
                if (ImGui::MenuItem("Material Editor"))
                {
                    //Want multiple mat edit windows at once! copy/adjust params from one or in relation to another (adjusting reflectivity of glass, and color of objects being reflect at once)
                    lastview = "Material Editor";
                };
                ImGui::MenuItem("Scenegraph");
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar;
            bool opened = false;

            ImGui::Begin("Hello, world!", &opened, flags);                          // Create a window called "Hello, world!" and append into it.
            ///*
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("Menu"))
                {
                    //IMGUI_DEMO_MARKER("Menu/File");
                    //ShowExampleMenuFile();
                    ImGui::MenuItem("Menu item");
                    if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
                    ImGui::Separator();

                    if (ImGui::BeginMenu("More.."))
                    {
                        ImGui::MenuItem("Hello");
                        ImGui::MenuItem("Sailor");
                        if (ImGui::BeginMenu("Recurse.."))
                        {
                            //ShowExampleMenuFile();
                            ImGui::MenuItem("Nested Menu item");
                            ImGui::EndMenu();
                        }
                        ImGui::EndMenu();
                    }

                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            //CLEARGLERR();
            ImGui::Text("Vendor:\t%s", glGetString(GL_VENDOR));
            ImGui::Text("Renderer:\t%s", glGetString(GL_RENDERER));
            ImGui::Text("Version:\t%s", glGetString(GL_VERSION));
            ImGui::Text("GLSL:\t%s", glGetString(GL_SHADING_LANGUAGE_VERSION));
            CLEARGLERR();

            if (shaderedit) {
                ImGui::Text("shader edit");
            }
            ImGui::Text(lastview.c_str());

            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);
            ImGui::Checkbox("Dialog class", &show_demo_dlg);
            ImGui::Checkbox("GUI launcher", &showguilauncher);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        ///*
        dialog dlg;
        if (show_demo_dlg) {
            dlg.doit(&show_demo_dlg);
        }
        //*/


        if (showguilauncher) {
            showlauncher(&showguilauncher);
        }

        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
