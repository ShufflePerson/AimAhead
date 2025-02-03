#include "../GUI.h"
static const size_t TabWidth = 85;
static const size_t TabHeight = 23;
static const size_t TabMargin = 10;

void gui::add_tab(size_t index, const char* text, size_t &currentTab) {
    ImGui::PushID(index);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1);

	if (currentTab == index)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(228, 13, 13));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor(228, 13, 13));
	}
	else {
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(30, 30, 30));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor(62, 62, 62));
	}

    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor(228, 13, 13));

    if (ImGui::Button(text, ImVec2(TabWidth, TabHeight)))
        currentTab = index;

    ImGui::PopStyleVar();
    ImGui::PopStyleColor(3);
    ImGui::PopID();
}

void gui::render_tabs(size_t& current_tab) {

	size_t num_tabs = 5;

	float total_tabs_width = (num_tabs * TabWidth) + ((num_tabs - 1) * TabMargin);
	float window_width = ImGui::GetWindowWidth();
	float start_x = (window_width - total_tabs_width) * 0.5f;

	ImGui::SetCursorPosX(start_x);

	add_tab(0, XorStr("Aimbot"), current_tab);
	ImGui::SameLine();
	add_tab(1, XorStr("Visuals"), current_tab);
	ImGui::SameLine();
	add_tab(2, XorStr("Misc"), current_tab);
	ImGui::SameLine();
	add_tab(3, XorStr("Margins"), current_tab);
	ImGui::SameLine();
	add_tab(4, XorStr("Settings"), current_tab);
}