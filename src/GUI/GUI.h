#pragma once
#define GLFW_EXPOSE_NATIVE_WIN32

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <Windows.h>
#include <format> 
#include <opencv2/opencv.hpp>

#include "../Structs/Config.h"
#include "../Input/Input.h"
#include "../Utils/Utils.h"
#include "../ModelManager/ModelManager.h"
#include "../ConfigManager/ConfigManager.h"
#include "./Preview/Preview.h"
#include "../Security/xor.h"
#include "../Security/Security.h"

namespace gui {

	void init_gui();
	void load_config(AimConfig* cfg);
	AimConfig* get_config();

	bool get_is_visible();

	void update_boxes(const std::vector<BoundingBox>& boxes);

	void __render__aimbot_tab__(AimConfig* config);
	void __render__visuals_tab__(AimConfig* config);
	void __render__misc_tab__(AimConfig* config);
	void __render__settings_tab__(AimConfig* config);
	void __render__preview_tab__(AimConfig* config);
}