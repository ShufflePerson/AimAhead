#include "../GUI.h"

void gui::__render__settings_tab__(AimConfig* config) {
	ImGui::NewLine();
	ImGui::Checkbox("Save Training Data", &config->b_save_training_data);
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip("Saves training data to the data folder.");
}