#include "../AimAheadUI.h"

void aimahead_ui::draw_visuals_tab(AimConfig *cfg) {
    ImGui::AH_Checkbox_Prop checkbox_prop = get_default_checkbox_prop();
    ImGui::AH_Slider_Prop slider_prop = get_default_slider_prop();
    ImGui::AH_ButtonInfo_Prop buttoninfo_prop = get_default_buttoninfo_prop();

    ImVec2 container_box_pos = draw_container_box(XorStr("ESP"));
    ImGui::SetCursorPosX(container_box_pos.x);
    ImGui::SetCursorPosY(container_box_pos.y);
    ImGui::AH_Checkbox(XorStr("Box ESP"), XorStr("Renders detected targets"), &cfg->b_draw_targets_on_screen, &checkbox_prop);
    ImGui::AH_Checkbox(XorStr("Show Fov"), XorStr("Renders A Fov Circle"), &cfg->b_draw_aim_fov, &checkbox_prop);
}