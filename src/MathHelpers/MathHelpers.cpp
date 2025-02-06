#include "MathHelpers.h"

namespace math_helpers {
    std::pair<double, double> calculate_mouse_movement(int xmin, int xmax, int ymin, int ymax, double sensitivity, double startX, double startY, EAimPosition aim_position, AimConfig* cfg) {
        double targetX = (xmin + xmax) / 2.0;
        double targetY = (ymin + ymax) / 2.0;

        double deltaX_raw = targetX - startX;
        double deltaY_raw = targetY - startY;
        double distance = std::sqrt(std::pow(targetX - startX, 2) + std::pow(targetY - startY, 2));


        if (aim_position == TOP) {
            double boxHeight = abs(ymax - ymin);
            double margin_from_top = boxHeight * (cfg->i_head_margin / static_cast<double>(100));
            if (margin_from_top <= cfg->f_minimum_top_margin) {
                margin_from_top = cfg->f_minimum_top_margin;
            }
            deltaY_raw = (ymin + margin_from_top) - startY;
        }

        if (aim_position == CHEST) {
            double boxHeight = abs(ymax - ymin);
            double margin_from_top = boxHeight * 0.35;
            deltaY_raw = (ymin + margin_from_top) - startY;
        }


        return std::make_pair(deltaX_raw * sensitivity, deltaY_raw * sensitivity);
    }

    double Kp_x = 0.19;
    double Kd_x = 0.0;
    double Kp_y = 0.19;
    double Kd_y = 0.0;

    std::pair<double, double> get_aim_position(int xmin, int xmax, int ymin, int ymax, double startX, double startY, EAimPosition aim_position, AimConfig* cfg) {
        double boxMiddleX = (xmin + xmax) / 2.0;
        double boxMiddleY = (ymin + ymax) / 2.0;
        double aim_position_X = boxMiddleX - startX;
        double aim_position_Y = boxMiddleY - startY;

        if (aim_position == TOP) {
            double boxHeight = abs(ymax - ymin);
            double margin_from_top = boxHeight * (cfg->i_head_margin / static_cast<double>(100));
            if (margin_from_top <= cfg->f_minimum_top_margin) {
                margin_from_top = cfg->f_minimum_top_margin;
            }
            aim_position_Y = (ymin + margin_from_top) - startY;
        }

        if (aim_position == CHEST) {
            double boxHeight = abs(ymax - ymin);
            double margin_from_top = boxHeight * 0.35;
            aim_position_Y = (ymin + margin_from_top) - startY;
        }
        return { aim_position_X, aim_position_Y };
    }

    std::pair<double, double> calculate_mouse_movement_pid(int xmin, int xmax, int ymin, int ymax, double sensitivity, double startX, double startY, std::pair<double, double> previous_error, double dt, std::pair<double, double> target_velocity, EAimPosition aim_position, AimConfig* cfg) {
        std::pair<double, double> aim_pos = get_aim_position(xmin, xmax, ymin, ymax, startX, startY, aim_position, cfg);
        std::pair<double, double> output = calculate_mouse_movement_from_given_point(aim_pos.first, aim_pos.second, previous_error, dt, target_velocity);
        output.first *= sensitivity;
        output.second *= sensitivity;

        return output;
    }

    std::pair<double, double> calculate_mouse_movement_from_given_point(double aim_position_X, double aim_position_Y, std::pair<double, double> previous_error, double dt, std::pair<double, double> target_velocity) {
        double derivative_x = (aim_position_X - previous_error.first) / dt;
        double derivative_y = (aim_position_Y - previous_error.second) / dt;
        double predicted_deltaX = aim_position_X + target_velocity.first * dt;
        double predicted_deltaY = aim_position_Y + target_velocity.second * dt;

        double prediction_weight = 0.4;
        double blended_deltaX = (1.0 - prediction_weight) * aim_position_X + prediction_weight * predicted_deltaX;
        double blended_deltaY = (1.0 - prediction_weight) * aim_position_Y + prediction_weight * predicted_deltaY;

        double output_x = Kp_x * blended_deltaX + Kd_x * derivative_x;
        double output_y = Kp_y * blended_deltaY + Kd_y * derivative_y;

        return { output_x, output_y };
    }

    std::pair<double, double> get_target_velocity(std::vector<std::pair<double, double>> history) {
        double velocityX = 1.0;
        double velocityY = 1.0;
        int total_size = history.size();
        if (total_size == 0) {
            return { velocityX, velocityY };
        }
        double xSum = 0.0;
        double ySum = 0.0;

        for (auto& current : history) {
            xSum += current.first;
            ySum += current.second;
        }

        velocityX = xSum / total_size;
        velocityY = ySum / total_size;

        return { velocityX, velocityY };
    }

    std::pair<double, double> calculate_new_frame_prediction_based_on_history(const std::vector<std::pair<double, double>> history, std::pair<double, double> current) {
        if (history.empty()) {
            return current;
        }

        if (history.size() == 1) {
            return { current.first + (current.first - history[0].first) * 0.1,
                    current.second + (current.second - history[0].second) * 0.1 };
        }

        std::pair<double, double> previous = history.back();
        std::pair<double, double> second_previous = (history.size() > 1) ? history[history.size() - 2] : previous; 

        double predicted_x = current.first + (current.first - previous.first);
        double predicted_y = current.second + (current.second - previous.second);

        return { predicted_x, predicted_y };
    }

    std::pair<double, double> calculate_future_frame_prediction(std::vector<std::pair<double, double>> history, std::pair<double, double> current, int frames_to_predict) {
        std::pair<double, double> predicted_frame = current;
        std::vector<std::pair<double, double>> current_history = history;

        for (int i = 0; i < frames_to_predict; ++i) {
            predicted_frame = calculate_new_frame_prediction_based_on_history(current_history, predicted_frame);
            current_history.push_back(predicted_frame);

            if (!current_history.empty()) {
                current_history.erase(current_history.begin());
            }
        }
        return predicted_frame;
    }

    double get_delta_between_positions(std::pair<double, double> old_pos, std::pair<double, double> new_pos) {
        double deltaX = abs(old_pos.first - new_pos.first);
        double deltaY = abs(old_pos.second - new_pos.second);

        double result = deltaX + deltaY;
        return result;
    }

    float squared_distance(float x1, float y1, float x2, float y2) {
        return std::pow(x1 - x2, 2) + std::pow(y1 - y2, 2);
    }

    std::pair<float, float> get_center(const BoundingBox& box) {
        float center_x = (box.xmin + box.xmax) / 2.0f;
        float center_y = (box.ymin + box.ymax) / 2.0f;
        return { center_x, center_y };
    }

    BoundingBox find_closests(std::vector<BoundingBox>& boxes) {
        float target_x = 320.0f;
        float target_y = 320.0f;
        float min_distance = 99999;
        BoundingBox closest_box = boxes[0];

        for (const auto& box : boxes) {
            std::pair<float, float> center = get_center(box);
            float dist = squared_distance(center.first, center.second, target_x, target_y);

            if (dist < min_distance) {
                min_distance = dist;
                closest_box = box;
            }
        }

        return closest_box;

    }

    bool is_point_inside_circle(double pointX, double pointY, double radius) {
        double centerX = 320;
        double centerY = 320;
        double distanceSquared = std::pow(pointX - centerX, 2) + std::pow(pointY - centerY, 2);

        double radiusSquared = std::pow(radius, 2);

        return distanceSquared <= radiusSquared;
    }
}