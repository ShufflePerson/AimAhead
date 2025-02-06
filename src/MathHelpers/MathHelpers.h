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
#include "../Structs/Vector2.h"

namespace math_helpers {
	Vector2 get_aim_position(int xmin, int xmax, int ymin, int ymax, EAimPosition aim_position, AimConfig* cfg);
	Vector2 get_center(const BoundingBox& box);
	double get_delta_between_positions(Vector2 old_pos, Vector2 new_pos);
	float squared_distance(float x1, float y1, float x2, float y2);

	Vector2 get_velocity(Vector2 first, Vector2 second, float dt);
	Vector2 get_avg_velocity(std::vector<Vector2> positions, float dt);
	Vector2 get_mouse_movement(Vector2 aim_cordinates, double sens, double dt);
	Vector2 get_aiming_error(Vector2 first, Vector2 second);

	BoundingBox find_closests(std::vector<BoundingBox>& boxes);
	bool is_point_inside_circle(double pointX, double pointY, double radius);
}