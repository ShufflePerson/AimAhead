#pragma once

enum ETargetMode {
	OBJECTIVE,
	CLOSEST
};

enum EAimPosition {
	TOP,
	CHEST,
	MIDDLE
};




struct AimConfig {
	bool b_aimbot;

	//Aim at all times
	bool b_always_aim;

	//Keybindings
	int k_aim_key = 0x02;
	int k_triggerbot_key = 0x58;

	//When an enemy is directly on the crosshair, start shooting automatically
	bool b_auto_trigger;
	int i_auto_trigger_delay;

	//Sensitivity
	double i_sensitivity;

	//Predicts where the enemy will be on the next frame using the previous frames
	bool b_predict_next_frame;
	//For how many frames should it gather data before making a prediction
	int i_frames_needed_for_prediction;
	//What is the max amount of frames to use to make a prediction
	int i_max_prediction_frames;

	//Sets how the target should be selected:
	//OBJECTIVE - Targets the target that has the highest confidence
	//CLOSEST - Targets the closests target to crosshair
	ETargetMode e_target_mode;

	//Sets where on the target to aim:
	//TOP - Aims at the head
	//MIDDLE - Aims at the torso
	EAimPosition e_aim_position;

	//Anti jitter ensures you wont be aiming left and right constantly if there are multiple enemies
	bool b_anti_target_jitter;
	//How many pixels away is it considered to jitter
	double d_maximum_jitter_amount;
	//How many frames to wait before switching to a new target
	int i_maximum_wait_time_for_target_reappearence;

	//Misc
	//If set to true, new training data will be automatically collected and saved to the data folder.
	bool b_save_training_data;
	//Only runs the AI model when the cursor is hidden (in-game)
	bool b_only_run_on_hidden_cursor;
	//Framerate cap
	int i_fps_cap;
	//Currently selected model
	int i_selected_model_index;

	//Visuals
	bool b_draw_targets_on_screen;
	
	//Margin from the top of the box
	int i_head_margin;

	//Read Only
	int read_only__i_fps;
};