#include "../GUI.h"
#define M_PI 3.14159

ImColor ColorRGB(float time) {
    float r = 0.5f + 0.5f * sin(time * 2.0f * M_PI / 3.0f); // Red component cycles over 3 seconds
    float g = 0.5f + 0.5f * sin(time * 2.0f * M_PI / 3.0f + 2.0f * M_PI / 3.0f); // Green, phase shifted
    float b = 0.5f + 0.5f * sin(time * 2.0f * M_PI / 3.0f + 4.0f * M_PI / 3.0f); // Blue, phase shifted
    return ImColor(r, g, b);
}


auto startTime = std::chrono::high_resolution_clock::now();
void gui::render_rgb_outline() {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 window_pos = ImGui::GetWindowPos();
    ImVec2 window_size = ImGui::GetWindowSize();
    float outline_thickness = 33.0f;
    float rounding_value = 8.0f;
    ImVec2 outline_rect_min = ImVec2(window_pos.x - outline_thickness, window_pos.y - outline_thickness);
    ImVec2 outline_rect_max = ImVec2(window_pos.x + window_size.x + outline_thickness, window_pos.y + window_size.y + outline_thickness);

    // Define the inner rectangle (same size and position as the window)
    ImVec2 inner_rect_min = window_pos;
    ImVec2 inner_rect_max = ImVec2(window_pos.x + window_size.x, window_pos.y + window_size.y);

    auto currentTime = std::chrono::high_resolution_clock::now();
    float elapsedTime = std::chrono::duration<float>(currentTime - startTime).count();
    ImColor outline_color = ColorRGB(elapsedTime);

    // 1. Draw the outer RGB rectangle
    draw_list->AddRectFilled(
        outline_rect_min,
        outline_rect_max,
        outline_color,
        rounding_value,
        ImDrawFlags_RoundCornersAll
    );

    // 2. Draw the inner rectangle with the window's background color
    ImColor window_bg_color = ImGui::GetColorU32(ImGuiCol_WindowBg); // Get window background color
    draw_list->AddRectFilled(
        inner_rect_min,
        inner_rect_max,
        window_bg_color,
        rounding_value,
        ImDrawFlags_RoundCornersAll
    );
}