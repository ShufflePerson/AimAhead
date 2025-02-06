#pragma once
#include <utility>
#include <complex>
#include <vector>
#include <cmath>
#include <utility>
#include <numeric>
#include <algorithm>
#include "../Structs/Config.h"
#include "../Structs/BoundingBox.h"

namespace math_helpers {
	std::pair<double, double> calculate_mouse_movement(int xmin, int xmax, int ymin, int ymax, double sensitivity, double startX, double startY, EAimPosition aim_position, AimConfig* cfg);
	std::pair<double, double> calculate_mouse_movement_pid(int xmin, int xmax, int ymin, int ymax, double sensitivity, double startX, double startY, std::pair<double, double> previous_error, double dt, std::pair<double, double> target_velocity, EAimPosition aim_position, AimConfig* cfg);
	std::pair<double, double> calculate_new_frame_prediction_based_on_history(std::vector<std::pair<double, double>> history, std::pair<double, double> current);
	std::pair<double, double> calculate_mouse_movement_from_given_point(double aim_position_X, double aim_position_Y, std::pair<double, double> previous_error, double dt, std::pair<double, double> target_velocity);
	std::pair<double, double> get_aim_position(int xmin, int xmax, int ymin, int ymax, double startX, double startY, EAimPosition aim_position, AimConfig* cfg);
	std::pair<float, float> get_center(const BoundingBox& box);
	std::pair<double, double> calculate_future_frame_prediction(std::vector<std::pair<double, double>> history, std::pair<double, double> current, int frames_to_predict);
	std::pair<double, double> get_target_velocity(std::vector<std::pair<double, double>> history);
	double get_delta_between_positions(std::pair<double, double> old_pos, std::pair<double, double> new_pos);
	float squared_distance(float x1, float y1, float x2, float y2);

	BoundingBox find_closests(std::vector<BoundingBox>& boxes);
	bool is_point_inside_circle(double pointX, double pointY, double radius);
}