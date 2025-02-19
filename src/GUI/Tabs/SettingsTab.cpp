#include "../GUI.h"

void gui::__render__settings_tab__(AimConfig* config) {
	ImGui::NewLine();
	ImGui::Checkbox(XorStr("Save Training Data"), &config->b_save_training_data);
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip(XorStr("Saves training data to the data folder."));

    ImGui::NewLine();
    ImGui::Checkbox(XorStr("Use new menu"), &config->b_use_new_gui);
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip(XorStr("Toggles between Legacy and New UI."));


    ImGui::NewLine();
    ImGui::Text(XorStr("Minimum Confidence"));
    ImGui::SameLine();
    ImGui::SetNextItemWidth(200);
    ImGui::SliderInt(XorStr("##min_confidence"), &config->i_minimum_confidence, 0, 100);
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip(XorStr("Only Targets that have a confidence level above the selected value will be processed."));


    std::vector<std::string> loaded_models = model_manager::get_loaded_models();

    ImGui::NewLine();
    ImGui::Text(XorStr("Select a model:"));
    ImGui::ListBox(
        XorStr("##models_list"),
        &config->i_selected_model_index,
        [](void* data, int idx, const char** out_text) {
            std::vector<std::string>* vec = (std::vector<std::string>*)data;
            if (idx < 0 || idx >= vec->size()) { return false; }
            *out_text = vec->at(idx).c_str();
            return true;
        },
        (void*)&loaded_models,
        loaded_models.size(),
        5
    );
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip(XorStr("Select a model from the models directory."));

    if (config->i_selected_model_index >= 0 && config->i_selected_model_index < loaded_models.size())
    {
        ImGui::Text(XorStr("Current Model: %s"), loaded_models[config->i_selected_model_index].c_str());
    }
}