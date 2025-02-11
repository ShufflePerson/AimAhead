#include "AimAheadUI.h"


void render_outline(ImVec2 topLeft, ImVec2 bottomRight, ImU32 color, float thickness = 1.0f) {
    // Get the current ImGui window's draw list
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // Ensure topLeft is actually the top-left and bottomRight is the bottom-right
    ImVec2 minPoint = ImVec2(ImMin(topLeft.x, bottomRight.x), ImMin(topLeft.y, bottomRight.y));
    ImVec2 maxPoint = ImVec2(ImMax(topLeft.x, bottomRight.x), ImMax(topLeft.y, bottomRight.y));

    // Add a rectangle outline to the draw list
    draw_list->AddRect(minPoint, maxPoint, color, 0.0f, 0, thickness);
}

void aimahead_ui::init_imgui_syles()
{
    ImGuiStyle& style = ImGui::GetStyle();

    style.WindowMinSize = ImVec2(160, 20);
    style.FramePadding = ImVec2(0, 0);
    style.WindowPadding = ImVec2(0, 0);
    style.WindowBorderSize = 0.0f;
    style.ItemSpacing = ImVec2(0, 0);
    style.ItemInnerSpacing = ImVec2(0,0);
    style.WindowRounding = 0.0f;
    style.FrameRounding = 0.0f;
    style.IndentSpacing = 6.0f;
    style.ItemInnerSpacing = ImVec2(0, 0);
    style.ColumnsMinSpacing = 0.0f;
    style.GrabMinSize = 14.0f;
    style.GrabRounding = 16.0f;
    style.ScrollbarSize = 12.0f;
    style.ScrollbarRounding = 16.0f;
    style.Alpha = 1.0f;

    style.Colors[ImGuiCol_Text] = ImVec4(0.86f, 0.93f, 0.89f, 0.78f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.86f, 0.93f, 0.89f, 0.28f);
    style.Colors[ImGuiCol_WindowBg] = colors.Dark;
    style.Colors[ImGuiCol_Border] = ImVec4(0.31f, 0.31f, 1.00f, 0.00f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.18f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.22f, 0.27f, 0.75f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.47f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.86f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.92f, 0.18f, 0.29f, 0.76f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.86f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_Separator] = ImVec4(0.14f, 0.16f, 0.19f, 1.00f);
    style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
    style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.92f, 0.18f, 0.29f, 0.43f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.9f);
}


bool v1 = false;
bool v2 = false;
bool v3 = false;
float column_size = 300.0f;
float sidebar_size = 100.0f;
void aimahead_ui::draw_ui_imgui()
{
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus;
    ImGui::Begin(XorStr("AimAhead"), nullptr, flags);
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 windowSize = ImGui::GetWindowSize();

    ImVec2 sidebarRectMin = ImGui::GetWindowPos();
    ImVec2 sidebarRectMax = ImVec2(sidebarRectMin.x + sidebar_size, sidebarRectMin.y + windowSize.y);
    ImU32 sidebarColor = ImGui::ColorConvertFloat4ToU32(colors.Sidebar);
    draw_list->AddRectFilled(sidebarRectMin, sidebarRectMax, sidebarColor);


    ImGui::AH_Checkbox_Prop checkbox_prop;
    checkbox_prop.f_available_x_space = column_size;
    checkbox_prop.primary_font = get_font(EFont::FONT_MEDIUM);
    
    ImVec2 container_box_x = draw_container_box("GENERAL");

    ImGui::AH_Checkbox("Test", "This is a test.", &v1, &checkbox_prop);
    ImGui::AH_Checkbox("Somethin", "Another option.", &v2, &checkbox_prop);
    ImGui::AH_Checkbox("Reality", "Does amazing thiings.", &v3, &checkbox_prop);

    ImGui::End();
}


int current_box = 1;
ImVec2 aimahead_ui::draw_container_box(const char* title) {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 windowSize = ImGui::GetWindowSize();
    float start_x = sidebar_size + 100.0f;
    ImVec2 box_size = ImVec2(222, 400);
    ImVec2 pos = ImVec2(start_x, (windowSize.y / 2) - (box_size.y / 2));
    ImVec2 br = ImVec2(pos.x + box_size.x, pos.y + box_size.y);

    draw_list->AddRectFilled(pos, br, ImGui::ColorConvertFloat4ToU32(colors.Box));
    render_outline(pos, br, IM_COL32(44, 44, 44, 0.49), 1.0);
    current_box++;

    return ImVec2(pos.x + 14, pos.y + 30);
}