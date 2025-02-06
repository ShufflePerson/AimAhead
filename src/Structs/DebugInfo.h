#pragma once


struct DebugInfo {
	bool b_have_active_target;
	int i_frames_since_target_was_seen;
	double target_velocity_x;
	double target_velocity_y;
	int i_prediction_frames_gathered;
};