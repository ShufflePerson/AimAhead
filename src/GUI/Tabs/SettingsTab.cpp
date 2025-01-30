#include "../GUI.h"

void gui::__render__settings_tab__(AimConfig* config) {
	ImGui::NewLine();
	ImGui::Checkbox("Save Training Data", &config->b_save_training_data);
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip("Saves training data to the data folder.");

    ImGui::NewLine();
    ImGui::Text("Minimum Confidence");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(200);
    ImGui::SliderInt("##min_confidence", &config->i_minimum_confidence, 0, 100);
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip("Only Targets that have a confidence level above the selected value will be processed.");


    std::vector<std::string> loaded_models = model_manager::get_loaded_models();

    ImGui::NewLine();
    ImGui::Text("Select a model:");
    ImGui::ListBox(
        "##models_list", 
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
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip("Select a model from the models directory.");

    if (config->i_selected_model_index >= 0 && config->i_selected_model_index < loaded_models.size())
    {
        ImGui::Text("Current Model: %s", loaded_models[config->i_selected_model_index].c_str());
    }
}