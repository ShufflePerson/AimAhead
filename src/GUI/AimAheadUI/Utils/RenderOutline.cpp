#include "../AimAheadUI.h"

void aimahead_ui::render_outline(ImVec2 topLeft, ImVec2 bottomRight, ImU32 color, float thickness) {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    ImVec2 minPoint = ImVec2(ImMin(topLeft.x, bottomRight.x), ImMin(topLeft.y, bottomRight.y));
    ImVec2 maxPoint = ImVec2(ImMax(topLeft.x, bottomRight.x), ImMax(topLeft.y, bottomRight.y));

    draw_list->AddRect(minPoint, maxPoint, color, 0.0f, 0, thickness);
}