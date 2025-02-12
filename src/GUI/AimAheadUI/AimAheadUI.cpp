#include "AimAheadUI.h"


aimahead_ui::TMenuSettings menu_settings;
bool b_menu_settings_init = false;


bool v1 = false;
bool v2 = false;
bool v3 = false;
float vv = 60.0f;
float vv2 = 0.0f;
float vv3 = 100.0f;
float vv4 = 50.0f;
void aimahead_ui::draw_ui_imgui(AimConfig *cfg)
{
    if (!b_menu_settings_init) {
        b_menu_settings_init = true;
        menu_settings.f_column_size = 250.0f;
        menu_settings.f_sidebar_width = 100.0f;
        menu_settings.i_current_box = 0;
        menu_settings.f_box_margin = 20.0f;
    }
    menu_settings.i_current_box = 0;
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus;
    ImGui::Begin(XorStr("AimAhead"), nullptr, flags);
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 windowSize = ImGui::GetWindowSize();

    draw_sidebar();

    ImGui::AH_Checkbox_Prop checkbox_prop;
    checkbox_prop.f_available_x_space = menu_settings.f_column_size - (menu_settings.f_box_margin * 2);
    checkbox_prop.primary_font = get_font(EFont::FONT_MEDIUM);

    ImGui::AH_Slider_Prop slider_prop;
    slider_prop.f_available_x_space = menu_settings.f_column_size - (menu_settings.f_box_margin * 2);
    slider_prop.primary_font = get_font(EFont::FONT_MEDIUM);
    slider_prop.circle_color = IM_COL32(166, 74, 201, 255);
    slider_prop.unselected_color = IM_COL32(82, 82, 82, 255);
    slider_prop.selected_color = IM_COL32(118, 56, 142, 255);

    ImGui::AH_ButtonInfo_Prop buttoninfo_prop;
    buttoninfo_prop.f_available_x_space = menu_settings.f_column_size - (menu_settings.f_box_margin * 2);
    buttoninfo_prop.primary_font = get_font(EFont::FONT_MEDIUM);
    buttoninfo_prop.button_background = IM_COL32(118, 56, 142, 255);
    
    ImVec2 container_box_pos = draw_container_box("GENERAL");
    ImGui::SetCursorPosX(container_box_pos.x);
    ImGui::SetCursorPosY(container_box_pos.y);
    ImGui::AH_Checkbox("Aimbot Enabled", "Enables Aimbot", &cfg->b_aimbot, &checkbox_prop);
    ImGui::AH_ButtonInfo("Aim Key", "Aimbot keybind", "Left", &v1, &buttoninfo_prop);
    ImGui::AH_Checkbox("Always Aim", "Aims at all times", &cfg->b_always_aim, &checkbox_prop);
    ImGui::AH_Slider("Sensitivity", &vv, 1.0f, 100.0f, "%", &slider_prop);
    ImGui::AH_Slider("Reaction Time", &vv2, 0.0f, 300.0f, "ms", & slider_prop);


    ImVec2 triggerbot_container = draw_container_box("TRIGGERBOT");
    ImGui::SetCursorPosX(triggerbot_container.x);
    ImGui::SetCursorPosY(triggerbot_container.y);
    ImGui::AH_Checkbox("Triggerbot", "Shoot automatically", &cfg->b_auto_trigger, &checkbox_prop);
    ImGui::AH_Checkbox("Adjust for recoil", "Helps with recoil control", &cfg->b_adjust_auto_trigger_for_recoil, &checkbox_prop);
    ImGui::AH_Checkbox("Precise", "Only shoots at selected position.", &v3, &checkbox_prop);
    ImGui::AH_Slider("Triggerbot Delay", &vv3, 0.0f, 300.0f, "ms", &slider_prop);
    ImGui::AH_Slider("Recoil Amount", &vv4, 50.0f, 300.0f, "%", &slider_prop);


    ImVec2 prediction_container = draw_container_box("PREDICTIONS");
    ImGui::SetCursorPosX(prediction_container.x);
    ImGui::SetCursorPosY(prediction_container.y);
    ImGui::AH_Checkbox("Predict", "Predicts the next frame.", &cfg->b_predict_next_frame, &checkbox_prop);

    ImGui::End();
}


ImVec2 aimahead_ui::draw_container_box(const char* title) {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 windowSize = ImGui::GetWindowSize();
    float start_x = menu_settings.f_sidebar_width + 20.0f + ((menu_settings.f_column_size + 30.0f) * menu_settings.i_current_box);
    ImVec2 box_size = ImVec2(menu_settings.f_column_size, 520);
    ImVec2 pos = ImVec2(start_x, (windowSize.y / 2) - (box_size.y / 2));
    ImVec2 br = ImVec2(pos.x + box_size.x, pos.y + box_size.y);
    ImVec2 starting_contents_pos = ImVec2(pos.x + menu_settings.f_box_margin, pos.y + 20);


    draw_list->AddRectFilled(pos, br, ImGui::ColorConvertFloat4ToU32(colors.Box));
    render_outline(pos, br, ImGui::ColorConvertFloat4ToU32(colors.Box_Stroke), 1.0);

    ImFont* font = get_font(EFont::FONT_MEDIUM);
    font->RenderText(draw_list, 18.0f, starting_contents_pos, IM_COL32(106, 106, 106, 255), ImVec4(0, 0, 2500, 2500), title, 0);
    
    menu_settings.i_current_box++;
    return ImVec2(starting_contents_pos.x, starting_contents_pos.y + 30);
}

void aimahead_ui::draw_sidebar() {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 windowSize = ImGui::GetWindowSize();
    
    ImVec2 sidebar_size = ImVec2(menu_settings.f_sidebar_width, windowSize.y);
    ImVec2 sidebar_pos = ImVec2(0, 0);

    draw_list->AddRectFilled(sidebar_pos, sidebar_size, ImGui::ColorConvertFloat4ToU32(colors.Sidebar));
    draw_list->AddLine(ImVec2(sidebar_size.x, 0), ImVec2(sidebar_size.x, sidebar_size.y), ImGui::ColorConvertFloat4ToU32(colors.Box_Stroke), 1.0f);
}