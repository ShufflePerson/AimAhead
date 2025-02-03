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


    std::pair<double, double> calculate_new_frame_prediction_based_on_history(std::vector<std::pair<double, double>> history, std::pair<double, double> current) {
        if (history.size() < 2) {
            return current;
        }

        double weightedDeltaXSum = 0.0;
        double weightedDeltaYSum = 0.0;
        double totalWeight = 0.0;
        int total_size = history.size();

        for (int i = 1; i < total_size; i++) {
            auto currentHistory = history[i];
            auto previousHistory = history[i - 1];
            double deltaX = currentHistory.first - previousHistory.first;
            double deltaY = currentHistory.second - previousHistory.second;

            double weight = (double)(total_size - i) / (total_size - 1);
            weightedDeltaXSum += deltaX * weight;
            weightedDeltaYSum += deltaY * weight;
            totalWeight += weight;
        }

        double deltaX = weightedDeltaXSum / totalWeight;
        double deltaY = weightedDeltaYSum / totalWeight;

        double predictedX = current.first + deltaX;
        double predictedY = current.second + deltaY;

        return std::pair<double, double>{predictedX, predictedY};
    }

    std::pair<double, double> calculate_future_frame_prediction(std::vector<std::pair<double, double>> history, std::pair<double, double> current, int frames_to_predict) {
        std::pair<double, double> predicted_frame = current;
        std::vector<std::pair<double, double>> current_history = history; 

        for (int i = 0; i < frames_to_predict; ++i) {
            predicted_frame = calculate_new_frame_prediction_based_on_history(current_history, predicted_frame);
            current_history.push_back(predicted_frame); 
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