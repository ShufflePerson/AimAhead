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

    auto last_config_save = std::chrono::system_clock::now();


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
        GLFWwindow* window = glfwCreateWindow(640, 640, XorStr("AimAhead"), NULL, NULL);
        if (window == NULL) {
            std::cout << XorStr("Failed to create GLFW window") << std::endl;
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
            std::cerr << XorStr("Failed to initialize global keybind.") << std::endl;
        }

        gladLoadGL();
        glViewport(0, 0, 640, 640);
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
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.18f);
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
        ImGui_ImplOpenGL3_Init(XorStr("#version 330"));
        toggle_window();
        toggle_window();

        size_t current_tab = 0;
        ImFontConfig fontcfg = {
        };
        ImFont* font_large = io.Fonts->AddFontFromFileTTF("./bin/font.ttf", 25.0f);
        if (font_large == nullptr) {
            font_large = io.Fonts->AddFontDefault();
        }
        io.FontGlobalScale = 0.70f;

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
                ImGui::Begin("1", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
                ImDrawList* draw_list = ImGui::GetWindowDrawList();
                ImU32 red_color = IM_COL32(243, 211, 11, 220);

                if (g_bounding_boxes.size() > 0) {
                    for (auto& box : g_bounding_boxes) {
                        ImVec2 p_min = ImVec2(box.xmin, box.ymin);
                        ImVec2 p_max = ImVec2(box.xmax, box.ymax);
                        draw_list->AddRect(p_min, p_max, red_color, 5.0f, ImDrawListFlags_AntiAliasedLines, 1.10f);

                        if (config->b_draw_confidence) {
                            float percentageValue = box.confidence * 100.0f;
                            int percentageInt = static_cast<int>(percentageValue);
                            std::stringstream ss;
                            ss << percentageInt;
                            std::string confidence_text = ss.str() + "%";
                            ImVec2 text_size = ImGui::CalcTextSize(confidence_text.c_str());
                            ImVec2 text_pos;
                            text_pos.x = (p_min.x + p_max.x) * 0.5f - text_size.x * 0.5f;
                            text_pos.y = p_min.y - text_size.y;

                            ImU32 text_color = IM_COL32_WHITE;
                            draw_list->AddText(text_pos, text_color, confidence_text.c_str());
                        }
                    }
                }
                ImVec2 center = ImVec2(320, 320);
                if (config->b_draw_aim_fov) {
                    draw_list->AddCircle(center, (float)config->i_fov_radius, red_color, 360);
                }
                ImGui::End();

            }
            else {
                ImGui::Begin(XorStr("Overlay"), nullptr, flags);
                //render_rgb_outline();
                ImGui::PushFont(font_large);
                ImGui::NewLine();


                render_tabs(current_tab);
                if (current_tab == 0) {
                    __render__aimbot_tab__(config);
                }
                if (current_tab == 1) {
                    __render__visuals_tab__(config);
                }
                if (current_tab == 2) {
                    __render__misc_tab__(config);
                }
                if (current_tab == 3) {
                    __render__preview_tab__(config);
                }
                if (current_tab == 4) {
                    __render__settings_tab__(config);
                }
                const char* fps_text = XorStr("FPS: %d");
                ImVec2 window_size = ImGui::GetWindowSize();
                ImVec2 text_size = ImGui::CalcTextSize(fps_text);
                ImGui::SetCursorPos(ImVec2(10, window_size.y - text_size.y - 10));
                ImGui::Text(fps_text, config->read_only__i_fps);
                ImGui::End();
                ImGui::PopFont();

                auto current_time = std::chrono::system_clock::now();
                auto time_diff = current_time - last_config_save;
                auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(time_diff);
                if (milliseconds > std::chrono::milliseconds(1000)) {
                    config_manager::save_config(get_config());
                }
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