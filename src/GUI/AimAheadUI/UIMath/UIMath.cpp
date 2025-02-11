#include "UIMath.h"

bool ui_math::is_point_inside_circle(ImVec2 point, ImVec2 circle_center, float circle_radius, float sides) {
    float distance_x = point.x - circle_center.x;
    float distance_y = point.y - circle_center.y;
    float distance_squared = distance_x * distance_x + distance_y * distance_y;

    if (sides <= 0.0f) {
        return distance_squared <= (circle_radius * circle_radius);
    }
    else {
        float angle = std::atan2(distance_y, distance_x);

        // Ensure the angle is within [0, 2*PI).  atan2 returns [-PI, PI].
        if (angle < 0.0f) {
            angle += 2.0f * M_PI; 
        }

        float side_angle = 2.0f * M_PI / sides;

        float nearest_side_angle = std::round(angle / side_angle) * side_angle;

        float edge_distance = circle_radius / std::cos(side_angle * 0.5f);

        float distance = std::sqrt(distance_squared);

        return distance <= edge_distance;
    }
}
