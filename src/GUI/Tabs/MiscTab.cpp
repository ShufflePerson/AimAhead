#include "../GUI.h"

void gui::__render__misc_tab__(AimConfig* config) {
	ImGui::NewLine();
	ImGui::Checkbox(XorStr("Only run when in-game"), &config->b_only_run_on_hidden_cursor);
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip("Only run the AI if the cursor is hidden, hence in-game.");

	ImGui::NewLine();
	ImGui::Checkbox(XorStr("Geforce Now Mode"), &config->b_geforce_now_mode);
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip("Aimbot only runs when Geforce Now is in focus. All inputs are sent towards Geforce Now directly.");

	ImGui::NewLine();
	ImGui::Checkbox(XorStr("Automatic Counter Strafe"), &config->b_counter_strafe);
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip("Automatically counter strafes.");


	ImGui::NewLine();
	ImGui::Text(XorStr("FPS Cap:"));
	ImGui::SameLine();
	ImGui::SetNextItemWidth(200);
	ImGui::SliderInt(XorStr("FPS Cap"), &config->i_fps_cap, 0, 240);
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip(XorStr("The FPS of the Aimbot will be capped at this. Lower value = less CPU & GPU usage.\n0 = Unlimited."));

	ImGui::NewLine();
	ImGui::Checkbox(XorStr("Predictions"), &config->b_predict_next_frame);
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip(XorStr("Attempts to predict where the target will be on the next frame."));
	if (config->b_predict_next_frame) {
		ImGui::Text(XorStr("i_frames_needed_for_prediction:"));
		ImGui::SameLine();
		ImGui::SetNextItemWidth(200);
		ImGui::SliderInt(XorStr("##i_frames_needed_for_prediction"), &config->i_frames_needed_for_prediction, 1, 60);
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip(XorStr("How many frames should it collect data in order to calculate the prediction."));

		ImGui::Text(XorStr("i_max_prediction_frames:"));
		ImGui::SameLine();
		ImGui::SetNextItemWidth(200);
		ImGui::SliderInt(XorStr("##i_max_prediction_frames"), &config->i_max_prediction_frames, 1, 240);
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip(XorStr("A cap on how many of the last frames to store to calculate the prediction."));

		ImGui::Text(XorStr("i_future_frames_to_predict:"));
		ImGui::SameLine();
		ImGui::SetNextItemWidth(200);
		ImGui::SliderInt(XorStr("##i_future_frames_to_predict"), &config->i_future_frames_to_predict, 1, 30);
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip(XorStr("How many frames into the future should it predict."));
	}



	double minVal = 1.0;
	double maxVal = 60.0;
	ImGui::NewLine();
	ImGui::Checkbox(XorStr("Anti Jitter"), &config->b_anti_target_jitter);
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip(XorStr("Ensures the Aimbot won't jitter back and forward between targets if more than one are visible."));
	if (config->b_anti_target_jitter) {
		ImGui::Text(XorStr("d_maximum_jitter_amount:"));
		ImGui::SameLine();
		ImGui::SetNextItemWidth(200);
		ImGui::SliderScalar(XorStr("##d_maximum_jitter_amount"), ImGuiDataType_Double, &config->d_maximum_jitter_amount, &minVal, &maxVal, XorStr("%.0f"));
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip(XorStr("The amount of pixels to the target that is considered a jitter."));

		ImGui::Text(XorStr("i_maximum_wait_time_for_target_reappearence:"));
		ImGui::SameLine();
		ImGui::SetNextItemWidth(200);
		ImGui::SliderInt(XorStr("##i_maximum_wait_time_for_target_reappearence"), &config->i_maximum_wait_time_for_target_reappearence, 1, 60);
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip(XorStr("How many frames to wait for the AI to pick up the target again before switching to a different target."));
	}




}