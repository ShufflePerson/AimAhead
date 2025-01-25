#include "../GUI.h"

void gui::__render__misc_tab__(AimConfig* config) {
	ImGui::NewLine();
	ImGui::Checkbox("Only run when in-game", &config->b_only_run_on_hidden_cursor);
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip("Only run the AI if the cursor is hidden, hence in-game.");

	ImGui::NewLine();
	ImGui::Text("FPS Cap:");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(200);
	ImGui::SliderInt("FPS Cap", &config->i_fps_cap, 0, 240);
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip("The FPS of the Aimbot will be capped at this. Lower value = less CPU & GPU usage.\n0 = Unlimited.");

	ImGui::NewLine();
	ImGui::Checkbox("Predictions", &config->b_predict_next_frame);
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip("Attempts to predict where the target will be on the next frame.");
	ImGui::Text("i_frames_needed_for_prediction:");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(200);
	ImGui::SliderInt("##i_frames_needed_for_prediction", &config->i_frames_needed_for_prediction, 1, 60);
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip("How many frames should it collect data in order to calculate the prediction.");

	ImGui::Text("i_max_prediction_frames:");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(200);
	ImGui::SliderInt("##i_max_prediction_frames", &config->i_max_prediction_frames, 1, 60);
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip("A cap on how many of the last frames to store to calculate the prediction.");


	double minVal = 1.0;
	double maxVal = 60.0;
	ImGui::NewLine();
	ImGui::Checkbox("Anti Jitter", &config->b_anti_target_jitter);
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip("Ensures the Aimbot won't jitter back and forward between targets if more than one are visible.");
	ImGui::Text("d_maximum_jitter_amount:");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(200);
	ImGui::SliderScalar("##d_maximum_jitter_amount", ImGuiDataType_Double, &config->d_maximum_jitter_amount, &minVal, &maxVal, "%.3f");
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip("The amount of pixels to the target that is considered a jitter.");

	ImGui::Text("i_maximum_wait_time_for_target_reappearence:");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(200);
	ImGui::SliderInt("##i_maximum_wait_time_for_target_reappearence", &config->i_maximum_wait_time_for_target_reappearence, 1, 60);
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip("How many frames to wait for the AI to pick up the target again before switching to a different target.");




}