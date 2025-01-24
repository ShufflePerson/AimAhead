#pragma once
#include <utility>
#include <complex>
#include <vector>
#include "../Structs/Config.h"
#include "../Structs/BoundingBox.h"

namespace math_helpers {
	std::pair<double, double> calculate_mouse_movement(int xmin, int xmax, int ymin, int ymax, double sensitivity, double startX, double startY, EAimPosition aim_position);
	std::pair<double, double> calculate_new_frame_prediction_based_on_history(std::vector<std::pair<double, double>> history, std::pair<double, double> current);
	std::pair<float, float> get_center(const BoundingBox& box);

	double get_delta_between_positions(std::pair<double, double> old_pos, std::pair<double, double> new_pos);
	float squared_distance(float x1, float y1, float x2, float y2);

	BoundingBox find_closests(std::vector<BoundingBox>& boxes);
}