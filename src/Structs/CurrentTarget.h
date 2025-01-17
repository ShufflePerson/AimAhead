#pragma once
#include <vector>

struct CurrentTarget {
	std::vector<std::pair<double, double>> v_location_history;
	std::pair<double, double> p_last_position;
	int i_last_seen_frame = 0;
	bool b_active = false;
};