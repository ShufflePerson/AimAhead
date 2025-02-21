#pragma once
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>
#include <glad/glad.h>
#include <iostream>
#include <Windows.h>
#include <format> 
#include <opencv2/opencv.hpp>
#include <filesystem>

#include "../../Security/xor.h"
#include "./UIMath/UIMath.h"
#include "../../Structs/Config.h"
#include "../../Injector/injector.h"

#define RGBA_TO_FLOAT(x, y, z) ImVec4(x/255, y/255, z/255, 1.0f)


constexpr uint32_t BOX_ESP_FLAG_FILLED = 1 << 0;

namespace aimahead_ui {
	enum EFont {
		FONT_MEDIUM = 100,
	};

	enum ETab {
		AIMBOT_TAB = 200,
		VISUALS_TAB = 300,
		MISC_TAB = 400,
		SETTINGS = 500
	};

	enum EBoxEsp {
		BOX_ESP_DEFAULT = 0,
		BOX_ESP_TRIANGLE = 1,
	};


	struct TColors {
		ImVec4 Dark = RGBA_TO_FLOAT(35, 35, 35);
		ImVec4 Box = RGBA_TO_FLOAT(13, 13, 13);
		ImVec4 Box_Stroke = ImGui::ColorConvertU32ToFloat4(IM_COL32(44, 44, 44, 255));
		ImVec4 Sidebar = RGBA_TO_FLOAT(44, 44, 44);
		ImVec4 WHITE = RGBA_TO_FLOAT(255, 255, 255);
	};
	static TColors colors;

	struct TImage {
		cv::Mat mat;
		GLuint textureID;
	};

	struct TMenuSettings {
		float f_column_size = 250.0f;
		float f_box_margin = 20.0f;
		float f_sidebar_width = 100.0f;
		int i_current_box = 0;
	};

	struct TTabItem {
		TImage image;
		TImage active_image;
		const char* text;
		ETab tab_type;
	};


	static TMenuSettings menu_settings;
	static bool b_menu_settings_init = false;

	ImGui::AH_Checkbox_Prop get_default_checkbox_prop();
	ImGui::AH_Slider_Prop get_default_slider_prop();
	ImGui::AH_ButtonInfo_Prop get_default_buttoninfo_prop();

	void init_imgui_syles();
	void draw_ui_imgui(AimConfig *cfg);
	
	ImVec2 draw_container_box(const char* title);
	void draw_sidebar();
	GLuint mat_to_texture(const cv::Mat& image);






	void load_fonts();
	ImFont* get_font(EFont font_type);

	void init_tabs();

	//Utils
	void render_outline(ImVec2 topLeft, ImVec2 bottomRight, ImU32 color, float thickness = 1.0f);
	TTabItem get_tab_item(const char* text, const char* image_path, ETab tab_id);

	//Tabs
	void draw_aimbot_tab(AimConfig* cfg);
	void draw_visuals_tab(AimConfig* cfg);
	void draw_misc_tab(AimConfig* cfg);
	void draw_settings_tab(AimConfig* cfg);

	void draw_esp_box(ImVec2 min, ImVec2 max, ImU32 col, EBoxEsp type, uint32_t flags);
	void draw_esp_box_default(ImVec2 min, ImVec2 max, ImU32 col, uint32_t flags);
	void draw_esp_box_triangle(ImVec2 min, ImVec2 max, ImU32 col, uint32_t flags);
}