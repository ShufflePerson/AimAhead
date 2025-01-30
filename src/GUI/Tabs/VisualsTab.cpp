#include "../GUI.h"

void gui::__render__visuals_tab__(AimConfig* config) {
	ImGui::NewLine();
	ImGui::Checkbox("Show Detected Targets", &config->b_draw_targets_on_screen);
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip("Renders a box around detected targets.");
	if (config->b_draw_targets_on_screen) {
		ImGui::Checkbox("Draw Confidence", &config->b_draw_confidence);
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip("Renders the confidence level on top of every box.");
	}
}