#pragma once
#include <cmath>
#include <algorithm>
#include <imgui.h>
#define M_PI 3.1415926535897

namespace ui_math {
	bool is_point_inside_circle(ImVec2 point, ImVec2 circle_center, float circle_radius, float sides);
}