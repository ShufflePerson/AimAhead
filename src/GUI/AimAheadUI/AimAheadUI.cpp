#include "AimAheadUI.h"




std::vector<aimahead_ui::TTabItem> tab_cache;
aimahead_ui::ETab active_tab = aimahead_ui::ETab::AIMBOT_TAB;


aimahead_ui::TTabItem aimahead_ui::get_tab_item(const char* text, const char* image_path, aimahead_ui::ETab tab_id) {
    std::string path_non_active = image_path;
    path_non_active += ".png";
    cv::Mat img_mat = cv::imread(path_non_active.c_str(), cv::IMREAD_UNCHANGED);
    if (img_mat.empty()) {
        printf(XorStr("[ERROR] (aimahead_ui::get_tab_item): The path %s is not valid."), image_path);
        return {};
    }

    std::string path_active = image_path;
    path_active += "_0.png";
    cv::Mat img_mat_active = cv::imread(path_active.c_str(), cv::IMREAD_UNCHANGED);
    if (img_mat_active.empty()) {
        printf(XorStr("[ERROR] (aimahead_ui::get_tab_item): The path %s is not valid."), image_path);
        return {};
    }


    GLuint texture_id = aimahead_ui::mat_to_texture(img_mat);
    aimahead_ui::TImage image_struct;
    image_struct.mat = img_mat;
    image_struct.textureID = texture_id;

    GLuint texture_id_2 = aimahead_ui::mat_to_texture(img_mat_active);
    aimahead_ui::TImage image_struct_2;
    image_struct_2.mat = img_mat_active;
    image_struct_2.textureID = texture_id_2;

    aimahead_ui::TTabItem tab_item;
    tab_item.image = image_struct;
    tab_item.active_image = image_struct_2;
    tab_item.text = text;
    tab_item.tab_type = tab_id;

    return tab_item;
}

void aimahead_ui::init_tabs() {
    tab_cache.push_back(get_tab_item("AIMBOT", "./bin/ui/0", ETab::AIMBOT_TAB));
    tab_cache.push_back(get_tab_item("VISUALS", "./bin/ui/1", ETab::VISUALS_TAB));
    tab_cache.push_back(get_tab_item("MISC", "./bin/ui/2", ETab::MISC_TAB));
    tab_cache.push_back(get_tab_item("SETTINGS", "./bin/ui/3", ETab::SETTINGS));
}



void aimahead_ui::draw_ui_imgui(AimConfig *cfg)
{
    if (!b_menu_settings_init) {
        menu_settings.f_column_size = 250.0f;
        menu_settings.f_sidebar_width = 100.0f;
        menu_settings.i_current_box = 0;
        menu_settings.f_box_margin = 20.0f;
        b_menu_settings_init = true;
    }
    menu_settings.i_current_box = 0;
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus;
    ImGui::Begin(XorStr("AimAhead"), nullptr, flags);
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 windowSize = ImGui::GetWindowSize();

    draw_sidebar();

    switch (active_tab)
    {
    case aimahead_ui::AIMBOT_TAB:
        draw_aimbot_tab(cfg);
        break;
    case aimahead_ui::VISUALS_TAB:
        draw_visuals_tab(cfg);
        break;
    case aimahead_ui::MISC_TAB:
        draw_misc_tab(cfg);
        break;
    case aimahead_ui::SETTINGS:
        draw_settings_tab(cfg);
        break;
    default:
        printf(XorStr("[ERROR AT aimahead_ui::draw_ui_imgui]: Invalid active_tab: %d\n"), active_tab);
        break;
    }
    
    const char* fps_text = XorStr("FPS: %d");
    ImVec2 window_size = ImGui::GetWindowSize();
    ImVec2 text_size = ImGui::CalcTextSize(fps_text);
    ImGui::SetCursorPos(ImVec2(window_size.x - text_size.x - 15.0f, window_size.y - text_size.y - 5.0f));
    ImGui::Text(fps_text, cfg->read_only__i_fps);

    ImGui::End();
}


ImVec2 aimahead_ui::draw_container_box(const char* title) {
    ImVec2 window_pos = ImGui::GetWindowPos();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 windowSize = ImGui::GetWindowSize();
    float start_x = menu_settings.f_sidebar_width + 20.0f + ((menu_settings.f_column_size + 30.0f) * menu_settings.i_current_box);
    ImVec2 box_size = ImVec2(menu_settings.f_column_size, 520);
    ImVec2 pos = ImVec2(start_x + window_pos.x, (windowSize.y / 2) - (box_size.y / 2) + window_pos.y);
    ImVec2 br = ImVec2(pos.x + box_size.x, pos.y + box_size.y);
    ImVec2 starting_contents_pos = ImVec2(pos.x + menu_settings.f_box_margin, pos.y + 20);


    draw_list->AddRectFilled(pos, br, ImGui::ColorConvertFloat4ToU32(colors.Box));
    render_outline(pos, br, ImGui::ColorConvertFloat4ToU32(colors.Box_Stroke), 1.0);

    ImFont* font = get_font(EFont::FONT_MEDIUM);
    font->RenderText(draw_list, 14.0f, starting_contents_pos, IM_COL32(106, 106, 106, 255), ImVec4(0, 0, 2500, 2500), title, 0);
    
    menu_settings.i_current_box++;
    return ImVec2(starting_contents_pos.x - window_pos.x, starting_contents_pos.y + 30 - window_pos.y);
}

void aimahead_ui::draw_sidebar() {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 windowSize = ImGui::GetWindowSize();
    
    ImVec2 sidebar_size = ImVec2(menu_settings.f_sidebar_width, windowSize.y);
    ImVec2 sidebar_pos = ImGui::GetWindowPos();
    draw_list->AddRectFilled(sidebar_pos, sidebar_size, ImGui::ColorConvertFloat4ToU32(colors.Sidebar));
    draw_list->AddLine(ImVec2(sidebar_size.x + sidebar_pos.x, sidebar_pos.y), ImVec2(sidebar_size.x + sidebar_pos.x, sidebar_size.y + sidebar_pos.y), ImGui::ColorConvertFloat4ToU32(colors.Box_Stroke), 1.0f);


    int i_total_tab_items = tab_cache.size();
    ImVec2 pos = ImVec2(0, 66);
    ImFont* font = get_font(EFont::FONT_MEDIUM);
    float font_size = 14.0f;
    float image_width = 22.0f;
    float sidebar_center_x = sidebar_size.x / 2;
    float item_padding = 20.0f;
    float active_tab_padding = 10.0f;
    for (const auto &tab : tab_cache) {
        ImU32 text_color = IM_COL32(226, 226, 226, 255);
        GLuint texture_id = tab.image.textureID;
        ImVec2 image_pos = ImVec2(sidebar_center_x - (image_width / 2) + sidebar_pos.x, pos.y + sidebar_pos.y);
        float image_ratio = tab.image.mat.cols / tab.image.mat.rows;
        ImVec2 image_size = ImVec2(image_width, image_ratio * image_width);
        pos.y += image_size.y + 10.0f;

        ImVec2 text_size = font->CalcTextSizeA(font_size, FLT_MAX, 0.0f, tab.text, 0);
        ImVec2 text_pos = ImVec2(sidebar_center_x - (text_size.x / 2) + sidebar_pos.x, pos.y + sidebar_pos.y);

        ImVec2 start_pos = ImVec2(sidebar_pos.x, image_pos.y - active_tab_padding);
        ImVec2 end_pos = ImVec2(sidebar_size.x + sidebar_pos.x, text_pos.y + text_size.y + active_tab_padding);

        ImRect full_bb = ImRect(start_pos, end_pos);
        pos.y += text_size.y;
        pos.y += item_padding;

        bool hovered = ImGui::IsMouseHoveringRect(full_bb.Min, full_bb.Max);
        if (hovered) {
            draw_list->AddRectFilled(start_pos, end_pos, IM_COL32(24, 24, 24, 255));

            if (ImGui::IsMouseClicked(0)) {
                active_tab = tab.tab_type;
            }
        }

        if (active_tab == tab.tab_type) {
            draw_list->AddRectFilled(start_pos, end_pos, IM_COL32(25, 25, 25, 255));
            text_color = IM_COL32(118, 56, 142, 255);
            texture_id = tab.active_image.textureID;
        }

        draw_list->AddImage((void*)(intptr_t)(texture_id), image_pos, ImVec2(image_size.x + image_pos.x, image_size.y + image_pos.y));
        font->RenderText(draw_list, font_size, text_pos, text_color, ImVec4(0, 0, 1000, 1000), tab.text, 0);
    }
}


void aimahead_ui::draw_esp_box(ImVec2 min, ImVec2 max, ImU32 col, EBoxEsp type, uint32_t flags) {
    if (type == EBoxEsp::BOX_ESP_DEFAULT) {
        draw_esp_box_default(min, max, col, flags);
        return;
    }
    if (type == EBoxEsp::BOX_ESP_TRIANGLE) {
        draw_esp_box_triangle(min, max, col, flags);
        return;
    }
}

void aimahead_ui::draw_esp_box_default(ImVec2 min, ImVec2 max, ImU32 col, uint32_t flags) {
    bool filled = flags & BOX_ESP_FLAG_FILLED;

    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    if (filled) {
        ImVec4 color_vec4 = ImGui::ColorConvertU32ToFloat4(col);
        color_vec4.w = 0.3;
        draw_list->AddRectFilled(min, max, ImGui::ColorConvertFloat4ToU32(color_vec4));
    }

    draw_list->AddRect(min, max, col);
}

void aimahead_ui::draw_esp_box_triangle(ImVec2 min, ImVec2 max, ImU32 col, uint32_t flags) {
    float stroke = 3.0f;
    float size = (max.x - min.x) / 3;
    bool filled = flags & BOX_ESP_FLAG_FILLED;

    ImDrawList* draw_list = ImGui::GetWindowDrawList();


    if (filled) {
        ImVec4 color_vec4 = ImGui::ColorConvertU32ToFloat4(col);
        color_vec4.w = 0.3;
        draw_list->AddRectFilled(min, max, ImGui::ColorConvertFloat4ToU32(color_vec4));
    }

    //TOP LEFT -> RIGHT
    draw_list->AddLine(ImVec2(min.x, min.y), ImVec2(min.x + size, min.y), col, stroke);
    //TOP LEFT -> DOWN
    draw_list->AddLine(ImVec2(min.x, min.y), ImVec2(min.x, min.y + size), col, stroke);

    //TOP RIGHT -> LEFT
    draw_list->AddLine(ImVec2(max.x, min.y), ImVec2(max.x - size, min.y), col, stroke);
    //TOP RIGHT -> DOWN
    draw_list->AddLine(ImVec2(max.x, min.y), ImVec2(max.x, min.y + size), col, stroke);


    //BOTTOM LEFT -> RIGHT
    draw_list->AddLine(ImVec2(min.x, max.y), ImVec2(min.x +size, max.y), col, stroke);
    //BOTTOM LEFT -> UP
    draw_list->AddLine(ImVec2(min.x, max.y), ImVec2(min.x, max.y - size), col, stroke);
    //BOTTOM RIGHT -> LEFT
    draw_list->AddLine(ImVec2(max.x, max.y), ImVec2(max.x - size, max.y), col, stroke);
    //BOTTOM RIGHT -> UP
    draw_list->AddLine(ImVec2(max.x, max.y), ImVec2(max.x, max.y - size), col, stroke);
}