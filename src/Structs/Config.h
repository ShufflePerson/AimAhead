#pragma once
#define CONFIG_ITERATION 8

enum ETargetMode {
	OBJECTIVE,
	CLOSEST
};

enum EAimPosition {
	TOP,
	CHEST,
	MIDDLE
};




//README - README - README
//Whenever a value is added/changed/removed, Utils/Utils.cpp & ConfigManager/ConfigManager.cpp also needs to be updated!
// Also the CONFIG_ITERATION macro needs to be incremented by 1
//README - README - README

struct AimConfig {
	int config_iteration = CONFIG_ITERATION;

	bool b_aimbot;

	//Aim at all times
	bool b_always_aim;

	//Keybindings
	int k_aim_key = 0x02;
	int k_triggerbot_key = 0x58;

	//When an enemy is directly on the crosshair, start shooting automatically
	bool b_auto_trigger;
	bool b_adjust_auto_trigger_for_recoil;
	int i_auto_trigger_recoil_adjustment_amount;
	int i_auto_trigger_recoil_adjustment_wait_time;
	int i_auto_trigger_delay;

	//Sensitivity
	double i_sensitivity;

	//Predicts where the enemy will be on the next frame using the previous frames
	bool b_predict_next_frame;
	//For how many frames should it gather data before making a prediction
	int i_frames_needed_for_prediction;
	//What is the max amount of frames to use to make a prediction
	int i_max_prediction_frames;
	//How many frames into the future should it predict
	int i_future_frames_to_predict;

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
	bool b_limit_fps;
	//Currently selected model
	int i_selected_model_index;
	//Minimum margin for TOP::EAimPosition
	float f_minimum_top_margin;
	//Minimum confidence level for a valid target
	int i_minimum_confidence;
	//Sends inputs directly to Geforce Now, only runs aimbot when GFN is in focus
	bool b_geforce_now_mode;
	//Automatic counter strafing
	bool b_counter_strafe;

	//Visuals
	bool b_draw_targets_on_screen;
	bool b_draw_confidence;
	
	//Margin from the top of the box
	int i_head_margin;

	bool b_aim_fov;
	bool b_draw_aim_fov;
	int i_fov_radius;

	//Colors
	unsigned int c_fov;
	unsigned int c_esp;
	
	//Model 
	bool b_int8_mode;

	//Read Only
	int read_only__i_fps;
};