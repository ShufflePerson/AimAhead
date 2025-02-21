#include "../AimAheadUI.h"

bool v1 = false;
bool v2 = false;
bool v3 = false;
float f_sens_cache = 0.1f;
float f_recoil_amount_cache = 0.1f;
float f_triggerbot_delay = 0.1f;
float vv2 = 0.0f;
float vv4 = 50.0f;
void aimahead_ui::draw_aimbot_tab(AimConfig *cfg) {
    if (f_sens_cache == 0.1f) {
        f_sens_cache = static_cast<float>(cfg->i_sensitivity);
        f_recoil_amount_cache = static_cast<float>(cfg->i_auto_trigger_recoil_adjustment_amount);
        f_triggerbot_delay = static_cast<float>(cfg->i_auto_trigger_delay);
    }

    ImGui::AH_Checkbox_Prop checkbox_prop = get_default_checkbox_prop();
    ImGui::AH_Slider_Prop slider_prop = get_default_slider_prop();
    ImGui::AH_ButtonInfo_Prop buttoninfo_prop = get_default_buttoninfo_prop();

    ImVec2 container_box_pos = draw_container_box(XorStr("GENERAL"));
    ImGui::SetCursorPosX(container_box_pos.x);
    ImGui::SetCursorPosY(container_box_pos.y);
    ImGui::AH_Checkbox(XorStr("Aimbot Enabled"), XorStr("Enables Aimbot"), &cfg->b_aimbot, &checkbox_prop);
    ImGui::AH_ButtonInfo(XorStr("Aim Key"), XorStr("Aimbot keybind"), XorStr("Left"), &v1, &buttoninfo_prop);
    ImGui::AH_Checkbox(XorStr("Always Aim"), XorStr("Aims at all times"), &cfg->b_always_aim, &checkbox_prop);
    if (ImGui::AH_Slider(XorStr("Sensitivity"), &f_sens_cache, 1.0f, 100.0f, "%", &slider_prop)) cfg->i_sensitivity = static_cast<int>(f_sens_cache);
    ImGui::AH_Slider(XorStr("Reaction Time"), &vv2, 0.0f, 300.0f, "ms", &slider_prop);


    ImVec2 triggerbot_container = draw_container_box(XorStr("TRIGGERBOT"));
    ImGui::SetCursorPosX(triggerbot_container.x);
    ImGui::SetCursorPosY(triggerbot_container.y);
    ImGui::AH_Checkbox(XorStr("Triggerbot"), XorStr("Shoot automatically"), &cfg->b_auto_trigger, &checkbox_prop);
    ImGui::AH_Checkbox(XorStr("Adjust for recoil"), XorStr("Helps with recoil control"), &cfg->b_adjust_auto_trigger_for_recoil, &checkbox_prop);
    ImGui::AH_Checkbox(XorStr("Precise"), XorStr("Only shoots at selected position."), &v3, &checkbox_prop);
    if (ImGui::AH_Slider(XorStr("Triggerbot Delay"), &f_triggerbot_delay, 0.0f, 300.0f, XorStr("ms"), &slider_prop)) cfg->i_auto_trigger_delay = static_cast<int>(f_triggerbot_delay);
    if (ImGui::AH_Slider(XorStr("Recoil Amount"), &f_recoil_amount_cache, 50.0f, 300.0f, XorStr("%"), &slider_prop)) cfg->i_auto_trigger_recoil_adjustment_amount = static_cast<int>(f_recoil_amount_cache);


    ImVec2 prediction_container = draw_container_box(XorStr("PREDICTIONS"));
    ImGui::SetCursorPosX(prediction_container.x);
    ImGui::SetCursorPosY(prediction_container.y);
    ImGui::AH_Checkbox(XorStr("Predict"), XorStr("Predicts the next frame."), &cfg->b_predict_next_frame, &checkbox_prop);
}