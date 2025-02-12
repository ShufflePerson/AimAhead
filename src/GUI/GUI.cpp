#include "GUI.h"

#define VK_W 0x57	
#define VK_A 0x41
#define VK_S 0x53
#define VK_D 0x44

#define USE_NEW_GUI

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
    uint32_t last_pressed_key = VK_INSERT;
    bool sent_counter_strafe = false;
    auto last_config_save = std::chrono::system_clock::now();

    DebugInfo* dbg_info;


    HWND get_hwnd() {
        return g_hwnd;
    }

    bool get_is_visible() {
        return g_is_visible;
    }

    static void toggle_window() {
        if (g_is_visible) {
            SetWindowLongPtr(g_hwnd, GWL_EXSTYLE, GetWindowLongPtr(g_hwnd, GWL_EXSTYLE) | WS_EX_TRANSPARENT | WS_EX_LAYERED);
            EnableWindow(g_hwnd, FALSE);
            g_is_visible = false;
        }
        else {
            SetWindowLongPtr(g_hwnd, GWL_EXSTYLE, GetWindowLongPtr(g_hwnd, GWL_EXSTYLE) & ~WS_EX_TRANSPARENT);
            SetWindowPos(g_hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
            EnableWindow(g_hwnd, TRUE);
            g_is_visible = true;
            ShowCursor(TRUE);
        }
    }

    LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
        if (nCode == HC_ACTION) {
            PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
            if (wParam == WM_KEYDOWN && p->vkCode == VK_INSERT) {
                toggle_window();
                return 1; 
            }

            if (p->vkCode == VK_W || p->vkCode == VK_A || p->vkCode == VK_S || p->vkCode == VK_D) {
                if (wParam == WM_KEYDOWN) {
                    last_pressed_key = p->vkCode;
                }
                if (wParam == WM_KEYUP) {
                    if (config->b_counter_strafe) {
                        if (!sent_counter_strafe) {
                            sent_counter_strafe = true;
                            if (p->vkCode == VK_W) {
                                input::send_key(VK_S);
                            }
                            if (p->vkCode == VK_S) {
                                input::send_key(VK_W);
                            }
                            if (p->vkCode == VK_A) {
                                input::send_key(VK_D);
                            }
                            if (p->vkCode == VK_D) {
                                input::send_key(VK_A);
                            }
                            sent_counter_strafe = false;
                        }

                    }
                }
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

    void set_debug_info_ptr(DebugInfo* ptr) {
        dbg_info = ptr;
    }


    void OutlinedText(ImVec2 pos, ImU32 col, const char* text, float strokeWidth, ImU32 strokeColor)
    {
        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        // Draw the stroke (outline)
        draw_list->AddText(NULL, 0, ImVec2(pos.x - strokeWidth, pos.y - strokeWidth), strokeColor, text);
        draw_list->AddText(NULL, 0, ImVec2(pos.x - strokeWidth, pos.y + strokeWidth), strokeColor, text);
        draw_list->AddText(NULL, 0, ImVec2(pos.x + strokeWidth, pos.y - strokeWidth), strokeColor, text);
        draw_list->AddText(NULL, 0, ImVec2(pos.x + strokeWidth, pos.y + strokeWidth), strokeColor, text);

        // Thinner diagonal strokes for better look (optional)
        draw_list->AddText(NULL, 0, ImVec2(pos.x - strokeWidth * 0.707f, pos.y - strokeWidth * 0.707f), strokeColor, text);
        draw_list->AddText(NULL, 0, ImVec2(pos.x - strokeWidth * 0.707f, pos.y + strokeWidth * 0.707f), strokeColor, text);
        draw_list->AddText(NULL, 0, ImVec2(pos.x + strokeWidth * 0.707f, pos.y - strokeWidth * 0.707f), strokeColor, text);
        draw_list->AddText(NULL, 0, ImVec2(pos.x + strokeWidth * 0.707f, pos.y + strokeWidth * 0.707f), strokeColor, text);

        // Draw the main text on top
        draw_list->AddText(NULL, 0, pos, col, text);
    }


    void init_gui() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
        glfwSwapInterval(1);
        GLFWwindow* window = glfwCreateWindow(950, 600, XorStr("AimAhead"), NULL, NULL);
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
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        io.IniFilename = nullptr;

#ifdef USE_NEW_GUI
        aimahead_ui::load_fonts();
        aimahead_ui::init_imgui_syles();
#endif

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(XorStr("#version 330"));
        toggle_window();
        toggle_window();

        size_t current_tab = 0;


        while (!glfwWindowShouldClose(window)) {
            if (!g_is_visible)
                glViewport(0, 0, 950, 600);
            else 
                glViewport(0, 0, 837, 527);
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus;
            if (!g_is_visible) {
                ImGui::SetNextWindowSize(ImVec2(640, 640));
                flags |= ImGuiWindowFlags_NoBackground;
                ImGui::Begin("1", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
                ImDrawList* draw_list = ImGui::GetWindowDrawList();

                if (g_bounding_boxes.size() > 0) {
                    for (auto& box : g_bounding_boxes) {
                        ImVec2 p_min = ImVec2(box.xmin, box.ymin);
                        ImVec2 p_max = ImVec2(box.xmax, box.ymax);
                        draw_list->AddRect(p_min, p_max, config->c_esp, 5.0f, ImDrawListFlags_AntiAliasedLines, 1.10f);

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
                if (config->b_draw_aim_fov && config->b_aim_fov) {
                    draw_list->AddCircle(center, (float)config->i_fov_radius, config->c_fov, 360);
                }

                if (dbg_info != nullptr) {
                    float strokeWidth = 1.0f; // Adjust as needed for outline thickness
                    ImU32 strokeColor = IM_COL32(0, 0, 0, 255); // Black outline

                    ImGui::PushStyleColor(ImGuiCol_Text, dbg_info->b_have_active_target ? IM_COL32(0, 255, 0, 255) : IM_COL32(255, 0, 0, 255));
                    ImVec2 text_pos = ImGui::GetCursorScreenPos();
                    OutlinedText(text_pos, ImGui::GetColorU32(ImGuiCol_Text), dbg_info->b_have_active_target ? "Have Active Target: TRUE" : "Have Active Target: FALSE", strokeWidth, strokeColor);
                    ImGui::Dummy(ImVec2(0.0f, ImGui::GetTextLineHeight()));  // Move cursor down
                    ImGui::PopStyleColor();

                    text_pos = ImGui::GetCursorScreenPos();
                    char buf[256]; // Adjust size as needed
                    sprintf(buf, "Frames since target was seen: %d", dbg_info->i_frames_since_target_was_seen);
                    OutlinedText(text_pos, ImGui::GetColorU32(ImGuiCol_Text), buf, strokeWidth, strokeColor);
                    ImGui::Dummy(ImVec2(0.0f, ImGui::GetTextLineHeight()));  // Move cursor down

                    text_pos = ImGui::GetCursorScreenPos();
                    sprintf(buf, "Target Velocity X: %.2f", dbg_info->target_velocity_x);
                    OutlinedText(text_pos, ImGui::GetColorU32(ImGuiCol_Text), buf, strokeWidth, strokeColor);
                    ImGui::Dummy(ImVec2(0.0f, ImGui::GetTextLineHeight()));  // Move cursor down

                    text_pos = ImGui::GetCursorScreenPos();
                    sprintf(buf, "Target Velocity Y: %.2f", dbg_info->target_velocity_y);
                    OutlinedText(text_pos, ImGui::GetColorU32(ImGuiCol_Text), buf, strokeWidth, strokeColor);
                    ImGui::Dummy(ImVec2(0.0f, ImGui::GetTextLineHeight()));  // Move cursor down

                    text_pos = ImGui::GetCursorScreenPos();
                    sprintf(buf, "Prediction Frames Gathered: %d", dbg_info->i_prediction_frames_gathered);
                    OutlinedText(text_pos, ImGui::GetColorU32(ImGuiCol_Text), buf, strokeWidth, strokeColor);
                    ImGui::Dummy(ImVec2(0.0f, ImGui::GetTextLineHeight()));  // Move cursor down

                }
                ImGui::End();

            }
            else {
                ImGui::SetNextWindowSize(ImVec2(950, 600));
#ifdef USE_NEW_GUI
                aimahead_ui::draw_ui_imgui(config);
#endif
#ifndef USE_NEW_GUI
                ImGui::Begin(XorStr("Overlay"), nullptr, flags);
                //render_rgb_outline();
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
#endif
                auto current_time = std::chrono::system_clock::now();
                auto time_diff = current_time - last_config_save;
                auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(time_diff);
                if (milliseconds > std::chrono::milliseconds(10000)) {
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