#include "../GUI.h"

static ImVec4 ImU32ToImVec4(ImU32 color)
{
	float r = ((color >> IM_COL32_R_SHIFT) & 0xFF) / 255.0f;
	float g = ((color >> IM_COL32_G_SHIFT) & 0xFF) / 255.0f;
	float b = ((color >> IM_COL32_B_SHIFT) & 0xFF) / 255.0f;
	float a = ((color >> IM_COL32_A_SHIFT) & 0xFF) / 255.0f;
	return ImVec4(r, g, b, a);
}

static ImU32 ImVec4ToImU32(const ImVec4& color)
{
	ImU32 r = (ImU32)(color.x * 255.0f + 0.5f) << IM_COL32_R_SHIFT;
	ImU32 g = (ImU32)(color.y * 255.0f + 0.5f) << IM_COL32_G_SHIFT;
	ImU32 b = (ImU32)(color.z * 255.0f + 0.5f) << IM_COL32_B_SHIFT;
	ImU32 a = (ImU32)(color.w * 255.0f + 0.5f) << IM_COL32_A_SHIFT;
	return r | g | b | a;
}

bool show_fov_color_changer = false;
bool show_esp_color_changer = false;
void gui::__render__visuals_tab__(AimConfig* config) {
	ImGui::NewLine();
	ImGui::Checkbox(XorStr("Draw Aimbot fov"), &config->b_draw_aim_fov);
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip(XorStr("Renders the radius of the Aimbot Fov."));
	if (config->b_draw_aim_fov) {
		ImVec4 color_vec4 = ImU32ToImVec4(config->c_fov);

		ImGui::Dummy(ImVec2(0, 3));
		if (ImGui::ColorButton("Fov Color", color_vec4, 0, ImVec2(50, 20))) {
			show_fov_color_changer = true;
		}
		ImGui::SameLine();
		ImGui::Text("Fov Color");

		if (show_fov_color_changer) {
			ImGui::SetNextWindowPos(ImGui::GetCursorScreenPos());
			ImGui::SetNextWindowFocus();
			ImGui::Begin("Fov Color Picker", &show_fov_color_changer, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
			ImGui::PushItemWidth(200);
			if (ImGui::ColorPicker4("##fov_picker", (float*)&color_vec4, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview))
			{
				config->c_fov = ImVec4ToImU32(color_vec4);
			}

			ImGui::End();
		}
	}

	ImGui::NewLine();
	ImGui::Checkbox(XorStr("Show Detected Targets"), &config->b_draw_targets_on_screen);
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip(XorStr("Renders a box around detected targets."));
	if (config->b_draw_targets_on_screen) {
		ImGui::Dummy(ImVec2(0, 3));
		ImGui::Checkbox(XorStr("Draw Confidence"), &config->b_draw_confidence);
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip(XorStr("Renders the confidence level on top of every box."));
	}
	if (config->b_draw_targets_on_screen) {
		ImVec4 color_vec4 = ImU32ToImVec4(config->c_esp);
		ImGui::Dummy(ImVec2(0, 3));
		if (ImGui::ColorButton("ESP Color", color_vec4, 0, ImVec2(50, 20))) {
			show_esp_color_changer = true;
		}
		ImGui::SameLine();
		ImGui::Text("ESP Color");

		if (show_esp_color_changer) {
			ImGui::SetNextWindowPos(ImGui::GetCursorScreenPos());
			ImGui::SetNextWindowFocus();
			ImGui::Begin("ESP Color Picker", &show_esp_color_changer, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
			ImGui::PushItemWidth(200);
			if (ImGui::ColorPicker4("##esp_picker", (float*)&color_vec4, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview))
			{
				config->c_esp = ImVec4ToImU32(color_vec4);
			}

			ImGui::End();
		}
	}
}