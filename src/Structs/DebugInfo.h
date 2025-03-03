#pragma once
#include "Vector2.h"
#include <vector>


struct DebugInfo {
	bool b_have_active_target;
	int i_frames_since_target_was_seen;
	double target_velocity_x;
	double target_velocity_y;
	int i_prediction_frames_gathered;
	std::vector<Vector2> history;
	Vector2 prediction;
	bool b_render_all_debug;
};