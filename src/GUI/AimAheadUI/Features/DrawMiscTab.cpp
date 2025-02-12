#include "../AimAheadUI.h"



void aimahead_ui::draw_misc_tab(AimConfig *cfg) {
    ImGui::AH_Checkbox_Prop checkbox_prop = get_default_checkbox_prop();
    ImGui::AH_Slider_Prop slider_prop = get_default_slider_prop();
    ImGui::AH_ButtonInfo_Prop buttoninfo_prop = get_default_buttoninfo_prop();


    ImVec2 container_box_pos = draw_container_box(XorStr("MISC"));
    ImGui::SetCursorPosX(container_box_pos.x);
    ImGui::SetCursorPosY(container_box_pos.y);
    ImGui::AH_Checkbox(XorStr("In-Game Check"), XorStr("Aimbot only runs in-game"), &cfg->b_only_run_on_hidden_cursor, &checkbox_prop);
    ImGui::AH_Checkbox(XorStr("Geforce Now Mode"), XorStr("Send inputs to GFN"), &cfg->b_geforce_now_mode, &checkbox_prop);
    ImGui::AH_Checkbox(XorStr("Counter Strafe"), XorStr("Instantly stop moving"), &cfg->b_counter_strafe, &checkbox_prop);

    ImVec2 aiming_box = draw_container_box(XorStr("AIMING"));
    ImGui::SetCursorPosX(aiming_box.x);
    ImGui::SetCursorPosY(aiming_box.y);
    ImGui::AH_Checkbox(XorStr("Anti Jitter"), XorStr("Avoids jittering"), &cfg->b_anti_target_jitter, &checkbox_prop);
}