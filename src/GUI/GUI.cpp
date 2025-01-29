#include "GUI.h"

namespace gui {
	AimConfig* config;
	void load_config(AimConfig* cfg) {
		config = cfg;
	}
	AimConfig* get_config() {
		return config;
	}
    HHOOK g_keyboardHook = nullptr;
    HWND g_hwnd = nullptr;
    std::vector<BoundingBox> g_bounding_boxes;
    bool g_is_visible = true;

    bool get_is_visible() {
        return g_is_visible;
    }

    static void toggle_window() {
        if (g_is_visible) {
            SetWindowLongPtr(g_hwnd, GWL_EXSTYLE, GetWindowLongPtr(g_hwnd, GWL_EXSTYLE) | WS_EX_TRANSPARENT | WS_EX_LAYERED);
            //SetLayeredWindowAttributes(g_hwnd, RGB(0, 0, 0), 0, LWA_ALPHA);
            EnableWindow(g_hwnd, FALSE);
            g_is_visible = false;
        }
        else {
            SetWindowLongPtr(g_hwnd, GWL_EXSTYLE, GetWindowLongPtr(g_hwnd, GWL_EXSTYLE) & ~WS_EX_TRANSPARENT);
            //SetLayeredWindowAttributes(g_hwnd, RGB(0, 0, 0), 255, LWA_ALPHA);
            SetWindowPos(g_hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
            EnableWindow(g_hwnd, TRUE);
            g_is_visible = true;
        }
    }

    LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
        if (nCode == HC_ACTION) {
            PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
            if (wParam == WM_KEYDOWN && p->vkCode == VK_INSERT) {
                toggle_window();
                return 1; 
            }
        }
        return CallNextHookEx(g_keyboardHook, nCode, wParam, lParam);
    }

    static bool initializeGlobalKeybind() {
        g_keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(nullptr), 0);
        return g_keyboardHook != nullptr;
    }

    void uninitializeGlobalKeybind() {
        if (g_keyboardHook) {
            UnhookWindowsHookEx(g_keyboardHook);
            g_keyboardHook = nullptr;
        }
    }

    void update_boxes(const std::vector<BoundingBox>& boxes) {
        g_bounding_boxes = boxes;
    }


    void init_gui() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
        GLFWwindow* window = glfwCreateWindow(800, 800, "AimAhead", NULL, NULL);
        if (window == NULL) {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return;
        }
        glfwMakeContextCurrent(window);

        std::pair<int, int> screen_size = utils::get_primary_display_size();
        int screenWidth = screen_size.first;
        int screenHeight = screen_size.second;
        int windowWidth = 640;
        int windowHeight = 640;
        glfwSetWindowPos(window, (screenWidth - windowWidth) / 2, (screenHeight - windowHeight) / 2);

        g_hwnd = glfwGetWin32Window(window);
        if (!initializeGlobalKeybind()) {
            std::cerr << "Failed to initialize global keybind." << std::endl;
        }

        gladLoadGL();
        glViewport(0, 0, 800, 800);
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        io.IniFilename = nullptr;

        ImGuiStyle& style = ImGui::GetStyle();

        style.WindowMinSize = ImVec2(160, 20);
        style.FramePadding = ImVec2(4, 2);
        style.ItemSpacing = ImVec2(6, 2);
        style.ItemInnerSpacing = ImVec2(6, 4);
        style.WindowRounding = 4.0f;
        style.FrameRounding = 2.0f;
        style.IndentSpacing = 6.0f;
        style.ItemInnerSpacing = ImVec2(2, 4);
        style.ColumnsMinSpacing = 50.0f;
        style.GrabMinSize = 14.0f;
        style.GrabRounding = 16.0f;
        style.ScrollbarSize = 12.0f;
        style.ScrollbarRounding = 16.0f;
        style.Alpha = 1.0f;

        style.Colors[ImGuiCol_Text] = ImVec4(0.86f, 0.93f, 0.89f, 0.78f);
        style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.86f, 0.93f, 0.89f, 0.28f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
        style.Colors[ImGuiCol_Border] = ImVec4(0.31f, 0.31f, 1.00f, 0.00f);
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.22f, 0.27f, 0.75f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.47f);
        style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
        style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_Button] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.86f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_Header] = ImVec4(0.92f, 0.18f, 0.29f, 0.76f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.86f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_Separator] = ImVec4(0.14f, 0.16f, 0.19f, 1.00f);
        style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
        style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
        style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
        style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_PlotLines] = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
        style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
        style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.92f, 0.18f, 0.29f, 0.43f);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.9f);


        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");
        toggle_window();
        toggle_window();

        while (!glfwWindowShouldClose(window)) {
            if (config == nullptr) continue;
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
            ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus;
            if (!g_is_visible) {
                flags |= ImGuiWindowFlags_NoBackground;

                if (g_bounding_boxes.size() > 0) {
                    ImGui::Begin("RedBoxOverlay", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
                    ImDrawList* draw_list = ImGui::GetWindowDrawList();

                    for (auto &box : g_bounding_boxes) {
                        ImVec2 p_min = ImVec2(box.xmin, box.ymin);
                        ImVec2 p_max = ImVec2(box.xmax, box.ymax);

                        ImU32 red_color = IM_COL32(255, 0, 0, 255);

                        draw_list->AddRect(p_min, p_max, red_color);
                    }
;

                    ImGui::End();
                }
            }
            else {
                ImGui::Begin("Overlay", nullptr, flags);
                if (ImGui::BeginTabBar("SettingsTabs")) {
                    if (ImGui::BeginTabItem("Aimbot")) {
                        __render__aimbot_tab__(config);
                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Visuals")) {
                        __render__visuals_tab__(config);
                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Misc")) {
                        __render__misc_tab__(config);
                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Settings")) {
                        __render__settings_tab__(config);
                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Margins")) {
                        __render__preview_tab__(config);
                        ImGui::EndTabItem();
                    }
                    ImGui::EndTabBar();
                }
                const char* fps_text = "FPS: %d";
                ImVec2 window_size = ImGui::GetWindowSize();
                ImVec2 text_size = ImGui::CalcTextSize(fps_text);
                ImGui::SetCursorPos(ImVec2(10, window_size.y - text_size.y - 10));
                ImGui::Text(fps_text, config->read_only__i_fps);
                ImGui::End();
            }



            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwDestroyWindow(window);

        uninitializeGlobalKeybind();
        glfwTerminate();
    }
}