#pragma once
#include <vector>
#include "./Vector2.h"

struct CurrentTarget {
	std::vector<Vector2> v_location_history;
	Vector2 p_last_position;
	int i_last_seen_frame = 0;
	bool b_active = false;
};