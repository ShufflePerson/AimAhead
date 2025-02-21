#include "MathHelpers.h"

namespace math_helpers {

    Vector2 get_aim_position(int xmin, int xmax, int ymin, int ymax,  EAimPosition aim_position, AimConfig* cfg) {
        double boxMiddleX = (xmin + xmax) / 2.0;
        double boxMiddleY = (ymin + ymax) / 2.0;
        double aim_position_X = boxMiddleX - dCENTER_AREA_X;
        double aim_position_Y = boxMiddleY - dCENTER_AREA_Y;

        if (aim_position == TOP) {
            double boxHeight = abs(ymax - ymin);
            double margin_from_top = boxHeight * (cfg->i_head_margin / static_cast<double>(100));
            if (margin_from_top <= cfg->f_minimum_top_margin) {
                margin_from_top = cfg->f_minimum_top_margin;
            }
            aim_position_Y = (ymin + margin_from_top) - dCENTER_AREA_Y;
        }
        else if (aim_position == CHEST) {
            double boxHeight = abs(ymax - ymin);
            double margin_from_top = boxHeight * 0.35;
            aim_position_Y = (ymin + margin_from_top) - dCENTER_AREA_Y;
        }
        return { aim_position_X + dCENTER_AREA_X, aim_position_Y + dCENTER_AREA_Y };
    }

    Vector2 get_velocity(Vector2 first, Vector2 second, float dt) {
        if (dt == 0.0f) {
            return { 0.0f, 0.0f };
        }
        return { (second.x - first.x) / dt, (second.y - first.y) / dt };
    }

    Vector2 get_avg_velocity(std::vector<Vector2> positions, float dt) {
        Vector2 sum = { 0.0, 0.0 };
        int count = positions.size();

        if (count <= 1) {
            return sum;
        }

        for (int i = 1; i < count; i++) {
            Vector2 velocity = get_velocity(positions[i], positions[i - 1], dt);
            sum.x += velocity.x;
            sum.y += velocity.y;
        }

        return {sum.x / count, sum.y / count};
    }


    double medium_sens = 0.65; 
    double high_sens = 0.55;
    double close_zone_radius = 20.0;
    double transition_zone_radius = 80.0;
    double max_movement = 35.0;
    double deadzone_threshold = 10.0;

    Vector2 get_mouse_movement(Vector2 aim_cordinates, double sens, double dt) {
        Vector2 mouse_movements;
        double distance = std::sqrt(std::pow(aim_cordinates.x - dCENTER_AREA_X, 2) + std::pow(aim_cordinates.y - dCENTER_AREA_Y, 2));
        double current_sens;

        if (distance <= close_zone_radius) {
            current_sens = high_sens;
        }
        else if (distance > transition_zone_radius) {
            current_sens = medium_sens;
        }
        else { 
            double t = (distance - close_zone_radius) / (transition_zone_radius - close_zone_radius);
            t = std::clamp(t, 0.0, 1.0);
            current_sens = high_sens * (1.0 - t) + medium_sens * t; 
        }

        mouse_movements.x = (aim_cordinates.x - dCENTER_AREA_X) * current_sens * dt * sens;
        mouse_movements.y = (aim_cordinates.y - dCENTER_AREA_Y) * current_sens * dt * sens;

        double movement_magnitude = std::sqrt(mouse_movements.x * mouse_movements.x + mouse_movements.y * mouse_movements.y);
        if (movement_magnitude > max_movement) {
            double scale_factor = max_movement / movement_magnitude;
            mouse_movements.x *= scale_factor;
            mouse_movements.y *= scale_factor;
        }

        if (distance <= close_zone_radius) {
            if (std::abs(aim_cordinates.x - dCENTER_AREA_X) < deadzone_threshold) {
                mouse_movements.x /= 2.0;
            }
            if (std::abs(aim_cordinates.y - dCENTER_AREA_Y) < deadzone_threshold) {
                mouse_movements.y /= 2.0;
            }
        }
        return mouse_movements;
    }

    double get_delta_between_positions(Vector2 old_pos, Vector2 new_pos) {
        double deltaX = abs(old_pos.x - new_pos.x);
        double deltaY = abs(old_pos.y - new_pos.y);

        double result = deltaX + deltaY;
        return result;
    }
    
    Vector2 get_aiming_error(Vector2 first, Vector2 second) {
        return {second.x - first.x, second.y - first.y};
    }

    float squared_distance(float x1, float y1, float x2, float y2) {
        return std::pow(x1 - x2, 2) + std::pow(y1 - y2, 2);
    }

    Vector2 get_center(const BoundingBox& box) {
        double center_x = (box.xmin + box.xmax) / 2.0f;
        double center_y = (box.ymin + box.ymax) / 2.0f;
        return { center_x, center_y };
    }

    BoundingBox find_closests(std::vector<BoundingBox>& boxes) {
        float target_x = fCENTER_AREA_X;
        float target_y = fCENTER_AREA_Y;
        float min_distance = 99999;
        BoundingBox closest_box = boxes[0];

        for (const auto& box : boxes) {
            Vector2 center = get_center(box);
            float dist = squared_distance(center.x, center.y, target_x, target_y);

            if (dist < min_distance) {
                min_distance = dist;
                closest_box = box;
            }
        }

        return closest_box;

    }

    bool is_point_inside_circle(double pointX, double pointY, double radius) {
        double distanceSquared = std::pow(pointX - dCENTER_AREA_X, 2) + std::pow(pointY - dCENTER_AREA_Y, 2);

        double radiusSquared = std::pow(radius, 2);

        return distanceSquared <= radiusSquared;
    }
}