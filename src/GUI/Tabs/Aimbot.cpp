#include "../GUI.h"


static const char* ETargetModeToString(ETargetMode value) {
	switch (value) {
	case ETargetMode::OBJECTIVE: return "Objective";
	case ETargetMode::CLOSEST: return "Closest";
	default: return "Unknown";
	}
}


static const char* eTargetModeNames[] = {
	ETargetModeToString(ETargetMode::OBJECTIVE),
	ETargetModeToString(ETargetMode::CLOSEST),
	nullptr
};

static const char* EAimPositionToString(EAimPosition value) {
	switch (value) {
	case EAimPosition::TOP: return "Head";
	case EAimPosition::CHEST: return "Chest";
	case EAimPosition::MIDDLE: return "Middle";
	default: return "Unknown";
	}
}


static const char* eAimPositionNames[] = {
	EAimPositionToString(EAimPosition::TOP),
	EAimPositionToString(EAimPosition::CHEST),
	EAimPositionToString(EAimPosition::MIDDLE),
	nullptr
};


void gui::__render__aimbot_tab__(AimConfig* config) {
	double minVal = 0.0;
	double maxVal = 1.0;

	//Aimbot
	ImGui::NewLine();
	ImGui::Checkbox("Aimbot", &config->b_aimbot);
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip("Enables aimbot.");
	ImGui::Text("Sensitivity:");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(200);
	ImGui::SliderScalar("##Sensitivity", ImGuiDataType_Double, &config->i_sensitivity, &minVal, &maxVal, "%.3f");
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip("The distance to target in pixels is multiplied by this value.");
	ImGui::Checkbox("Always Aim", &config->b_always_aim);
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip("Always aim without having to hold down the aim key.");


	ImGui::Text("Target Selection Mode");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(200);
	if (ImGui::BeginCombo("##TargetSelectionMode", ETargetModeToString(config->e_target_mode)))
	{
		for (int i = 0; eTargetModeNames[i] != nullptr; ++i)
		{
			bool isSelected = (config->e_target_mode == static_cast<ETargetMode>(i)); 
			if (ImGui::Selectable(eTargetModeNames[i], isSelected))
			{
				config->e_target_mode = static_cast<ETargetMode>(i);
			}
			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip("Objective - Chooses the target in which the AI is the most confident in.\nClosest - Chooses the target which is the closest to the crosshair.");


	ImGui::Text("Aim Position");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(200);
	if (ImGui::BeginCombo("##AimPosition", EAimPositionToString(config->e_aim_position)))
	{
		for (int i = 0; eAimPositionNames[i] != nullptr; ++i)
		{
			bool isSelected = (config->e_target_mode == static_cast<EAimPosition>(i));
			if (ImGui::Selectable(eAimPositionNames[i], isSelected))
			{
				config->e_aim_position = static_cast<EAimPosition>(i);
			}
			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip("Head - Aims at the top of the box.\nChest - Aims at the top of the box with a 35 percent margin.\nMiddle - Aims at the middle of the box.");


	//Triggerbot
	ImGui::NewLine();
	ImGui::Checkbox("Triggerbot", &config->b_auto_trigger);
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip("When the Triggerbot key is being held down, it will aim at the target and start shooting once the target is on the crosshair.");
	ImGui::Text("Delay:");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(200);
	ImGui::SliderInt("##Delay", &config->i_auto_trigger_delay, 0, 500);
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip("A millisecond amount to wait before shooting at the target.");

}