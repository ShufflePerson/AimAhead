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
    bool g_is_visible = true;

    void toggle_window() {
        if (g_is_visible) {
            SetWindowLongPtr(g_hwnd, GWL_EXSTYLE, GetWindowLongPtr(g_hwnd, GWL_EXSTYLE) | WS_EX_TRANSPARENT | WS_EX_LAYERED);
            SetLayeredWindowAttributes(g_hwnd, RGB(0, 0, 0), 0, LWA_ALPHA);
            g_is_visible = false;
        }
        else {
            SetWindowLongPtr(g_hwnd, GWL_EXSTYLE, GetWindowLongPtr(g_hwnd, GWL_EXSTYLE) & ~WS_EX_TRANSPARENT);
            SetLayeredWindowAttributes(g_hwnd, RGB(0, 0, 0), 255, LWA_ALPHA);
            SetWindowPos(g_hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
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

    bool InitializeGlobalKeybind() {
        g_keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(nullptr), 0);
        return g_keyboardHook != nullptr;
    }

    void UninitializeGlobalKeybind() {
        if (g_keyboardHook) {
            UnhookWindowsHookEx(g_keyboardHook);
            g_keyboardHook = nullptr;
        }
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

        int screenWidth = 3440;
        int screenHeight = 1440;
        int windowWidth = 800;
        int windowHeight = 800;
        glfwSetWindowPos(window, (screenWidth - windowWidth) / 2, (screenHeight - windowHeight) / 2);

        g_hwnd = glfwGetWin32Window(window);
        if (InitializeGlobalKeybind()) {
            std::cout << "Global keybind (Insert) initialized." << std::endl;
        }
        else {
            std::cerr << "Failed to initialize global keybind." << std::endl;
        }

        gladLoadGL();
        glViewport(0, 0, 800, 800);
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");
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
            }

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
                ImGui::EndTabBar();
            }
            const char* fps_text = "FPS: %d";
            ImVec2 window_size = ImGui::GetWindowSize();
            ImVec2 text_size = ImGui::CalcTextSize(fps_text);
            ImGui::SetCursorPos(ImVec2(10, window_size.y - text_size.y - 10)); 
            ImGui::Text(fps_text, config->read_only__i_fps);

            ImGui::End();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwDestroyWindow(window);

        UninitializeGlobalKeybind();
        glfwTerminate();
    }
}