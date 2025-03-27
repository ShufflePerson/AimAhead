#include "../AimAheadUI.h"


void aimahead_ui::draw_aimbot_tab(AimConfig *cfg) {
	static bool bAimKeyButtonPressed = false;
    static bool bPreciseTargeting = false;
    static float fSens = 0.1f;
    static float fRecoilAmount = 0.1f;
    static float fTriggerbotDelay = 0.1f;
    static float fFovRadius = 0.1f;
    static float fReactionTime = 0.0f;


    if (fSens == 0.1f) {
        fSens = static_cast<float>(cfg->i_sensitivity);
        fRecoilAmount = static_cast<float>(cfg->i_auto_trigger_recoil_adjustment_amount);
        fTriggerbotDelay = static_cast<float>(cfg->i_auto_trigger_delay);
    }
    
    if (fFovRadius == 0.1f) {
        fFovRadius = static_cast<float>(cfg->i_fov_radius);
    }

    ImGui::AH_Checkbox_Prop checkbox_prop = get_default_checkbox_prop();
    ImGui::AH_Slider_Prop slider_prop = get_default_slider_prop();
    ImGui::AH_ButtonInfo_Prop buttoninfo_prop = get_default_buttoninfo_prop();

    ImVec2 container_box_pos = draw_container_box(XorStr("GENERAL"));
    ImGui::SetCursorPosX(container_box_pos.x);
    ImGui::SetCursorPosY(container_box_pos.y);
    ImGui::AH_Checkbox(XorStr("Aimbot Enabled"), XorStr("Enables Aimbot"), &cfg->b_aimbot, &checkbox_prop);
    ImGui::AH_ButtonInfo(XorStr("Aim Key"), XorStr("Aimbot keybind"), XorStr("Left"), &bAimKeyButtonPressed, &buttoninfo_prop);
    ImGui::AH_Checkbox(XorStr("Always Aim"), XorStr("Aims at all times"), &cfg->b_always_aim, &checkbox_prop);
    if (ImGui::AH_Slider(XorStr("Sensitivity"), &fSens, 1.0f, 100.0f, "%", &slider_prop)) cfg->i_sensitivity = static_cast<int>(fSens);
    ImGui::AH_Slider(XorStr("Reaction Time"), &fReactionTime, 0.0f, 300.0f, "ms", &slider_prop);


    ImGui::AH_Checkbox(XorStr("Fov"), XorStr("Only aim inside the fov"), &cfg->b_aim_fov, &checkbox_prop);
    if (ImGui::AH_Slider(XorStr("Fov Radius"), &fFovRadius, 1.0f, 360.0f, "px", &slider_prop)) cfg->i_fov_radius = static_cast<int>(fFovRadius);


    ImVec2 triggerbot_container = draw_container_box(XorStr("TRIGGERBOT"));
    ImGui::SetCursorPosX(triggerbot_container.x);
    ImGui::SetCursorPosY(triggerbot_container.y);
    ImGui::AH_Checkbox(XorStr("Triggerbot"), XorStr("Shoot automatically"), &cfg->b_auto_trigger, &checkbox_prop);
    ImGui::AH_Checkbox(XorStr("Adjust for recoil"), XorStr("Helps with recoil control"), &cfg->b_adjust_auto_trigger_for_recoil, &checkbox_prop);
    ImGui::AH_Checkbox(XorStr("Precise"), XorStr("Only shoots at selected position."), &bPreciseTargeting, &checkbox_prop);
    if (ImGui::AH_Slider(XorStr("Triggerbot Delay"), &fTriggerbotDelay, 0.0f, 300.0f, XorStr("ms"), &slider_prop)) cfg->i_auto_trigger_delay = static_cast<int>(fTriggerbotDelay);
    if (ImGui::AH_Slider(XorStr("Recoil Amount"), &fRecoilAmount, 50.0f, 300.0f, XorStr("%"), &slider_prop)) cfg->i_auto_trigger_recoil_adjustment_amount = static_cast<int>(fRecoilAmount);


    ImVec2 prediction_container = draw_container_box(XorStr("PREDICTIONS"));
    ImGui::SetCursorPosX(prediction_container.x);
    ImGui::SetCursorPosY(prediction_container.y);
    ImGui::AH_Checkbox(XorStr("Predict"), XorStr("Predicts the next frame."), &cfg->b_predict_next_frame, &checkbox_prop);
}