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

#define RGBA_TO_FLOAT(x, y, z) ImVec4(x/255, y/255, z/255, 1.0f)


namespace aimahead_ui {
	void init_imgui_syles();
	void draw_ui_imgui();
	
	ImVec2 draw_container_box(const char* title);

	GLuint mat_to_texture(const cv::Mat& image);

	enum EFont {
		FONT_MEDIUM = 100,
	};
	void load_fonts();
	ImFont* get_font(EFont font_type);

	struct TColors {
		ImVec4 Dark = RGBA_TO_FLOAT(20, 20, 20);
		ImVec4 Box = RGBA_TO_FLOAT(13, 13, 13);
		ImVec4 Box_Stroke = ImGui::ColorConvertU32ToFloat4(IM_COL32(44, 44, 44, 0.49));
		ImVec4 Sidebar = RGBA_TO_FLOAT(44, 44, 44);
	};
	static TColors colors;

	struct TImage {
		cv::Mat mat;
		GLuint textureID;
	};
}