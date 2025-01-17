#pragma once
#define GLFW_EXPOSE_NATIVE_WIN32

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <Windows.h>

#include "../Structs/Config.h"


namespace gui {
	void init_gui();
	void load_config(AimConfig* cfg);
	AimConfig* get_config();


	void __render__aimbot_tab__(AimConfig* config);
	void __render__visuals_tab__(AimConfig* config);
	void __render__misc_tab__(AimConfig* config);
	void __render__settings_tab__(AimConfig* config);
}