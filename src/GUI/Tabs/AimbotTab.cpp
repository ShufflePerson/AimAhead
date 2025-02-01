#include "../GUI.h"


static const char* ETargetModeToString(ETargetMode value) {
	switch (value) {
	case ETargetMode::OBJECTIVE: return XorStr("Objective");
	case ETargetMode::CLOSEST: return XorStr("Closest");
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
	case EAimPosition::TOP: return XorStr("Head");
	case EAimPosition::CHEST: return XorStr("Chest");
	case EAimPosition::MIDDLE: return XorStr("Middle");
	default: return "Unknown";
	}
}


static const char* eAimPositionNames[] = {
	EAimPositionToString(EAimPosition::TOP),
	EAimPositionToString(EAimPosition::CHEST),
	EAimPositionToString(EAimPosition::MIDDLE),
	nullptr
};


bool b_aim_key_clicked = false;
DWORD last_key = 0;
void gui::__render__aimbot_tab__(AimConfig* config) {
	double minVal = 0.0;
	double maxVal = 1.0;

	//Aimbot
	ImGui::NewLine();
	ImGui::Checkbox(XorStr("Aimbot"), &config->b_aimbot);
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip(XorStr("Enables aimbot."));

	if (config->b_aimbot) {
		ImGui::Text(XorStr("Aim Key:"));
		ImGui::SameLine();
		if (b_aim_key_clicked) {
			ImGui::Button(XorStr("Press a key!"));
			DWORD new_key = input::get_last_pressed_key();
			if (last_key != new_key) {
				config->k_aim_key = new_key;
				last_key = new_key;
				b_aim_key_clicked = false;
			}
		}
		else {
			if (ImGui::Button(input::get_vk_string(config->k_aim_key).c_str())) {
				b_aim_key_clicked = true;
				last_key = input::get_last_pressed_key();
			}
		}

		ImGui::Text(XorStr("Sensitivity:"));
		ImGui::SameLine();
		ImGui::SetNextItemWidth(200);
		ImGui::SliderScalar(XorStr("##Sensitivity"), ImGuiDataType_Double, &config->i_sensitivity, &minVal, &maxVal, XorStr("%.3f"));
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip(XorStr("The distance to target in pixels is multiplied by this value."));


		ImGui::Text(XorStr("Target Selection Mode"));
		ImGui::SameLine();
		ImGui::SetNextItemWidth(200);
		if (ImGui::BeginCombo(XorStr("##TargetSelectionMode"), ETargetModeToString(config->e_target_mode)))
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
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip(XorStr("Objective - Chooses the target in which the AI is the most confident in.\nClosest - Chooses the target which is the closest to the crosshair."));


		ImGui::Text(XorStr("Aim Position"));
		ImGui::SameLine();
		ImGui::SetNextItemWidth(200);
		if (ImGui::BeginCombo(XorStr("##AimPosition"), EAimPositionToString(config->e_aim_position)))
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
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip(XorStr("Head - Aims at the top of the box.\nChest - Aims at the top of the box with a 35 percent margin.\nMiddle - Aims at the middle of the box."));
	}

	ImGui::NewLine();
	ImGui::Checkbox(XorStr("Always Aim"), &config->b_always_aim);
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip(XorStr("Always aim without having to hold down the aim key."));



	//Triggerbot
	ImGui::NewLine();
	ImGui::Checkbox("Triggerbot", &config->b_auto_trigger);
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip(XorStr("When the Triggerbot key is being held down, it will aim at the target and start shooting once the target is on the crosshair."));

	if (config->b_auto_trigger) {

		ImGui::Text(XorStr("Delay:"));
		ImGui::SameLine();
		ImGui::SetNextItemWidth(200);
		ImGui::SliderInt(XorStr("##Delay"), &config->i_auto_trigger_delay, 0, 500);
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip(XorStr("A millisecond amount to wait before shooting at the target."));


		ImGui::Checkbox(XorStr("Adjust Triggerbot for Recoil"), &config->b_adjust_auto_trigger_for_recoil);
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip(XorStr("When Triggerbot is shooting, it will increase the Y sensitivity by adjustment amount % to account for recoil."));

		if (config->b_adjust_auto_trigger_for_recoil) {
			ImGui::Text(XorStr("Triggerbot Recoil Adjustment Amount:"));
			ImGui::SameLine();
			ImGui::SetNextItemWidth(200);
			ImGui::SliderInt(XorStr("##triggerbot_adjustment_amount"), &config->i_auto_trigger_recoil_adjustment_amount, 100, 400);
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip("The percentege will be applied to the Y sensitivity after triggerbot has been shooting for X amount of time.");

			ImGui::Text(XorStr("Triggerbot Recoil Adjustment Wait Time:"));
			ImGui::SameLine();
			ImGui::SetNextItemWidth(200);
			ImGui::SliderInt(XorStr("##triggerbot_adjustment_wait_time"), &config->i_auto_trigger_recoil_adjustment_wait_time, 0, 1000);
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip(XorStr("How long should the triggerbot shoot for before kicking in the recoil adjustment in milliseconds."));

		}
	}



}