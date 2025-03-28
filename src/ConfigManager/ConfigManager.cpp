#include "ConfigManager.h"


namespace config_manager {
    const std::string configFilePath = XorStr("config.cfg");

    bool serialize_aim_config(const AimConfig& config, const std::string& filePath) {
        std::ofstream file(filePath, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << XorStr("Error opening file for writing: ") << filePath << std::endl;
            return false;
        }
        file.write(reinterpret_cast<const char*>(&config.config_iteration), sizeof(config.config_iteration));
        file.write(reinterpret_cast<const char*>(&config.b_aimbot), sizeof(config.b_aimbot));
        file.write(reinterpret_cast<const char*>(&config.b_always_aim), sizeof(config.b_always_aim));
        file.write(reinterpret_cast<const char*>(&config.k_aim_key), sizeof(config.k_aim_key));
        file.write(reinterpret_cast<const char*>(&config.k_triggerbot_key), sizeof(config.k_triggerbot_key));
        file.write(reinterpret_cast<const char*>(&config.b_auto_trigger), sizeof(config.b_auto_trigger));
        file.write(reinterpret_cast<const char*>(&config.b_adjust_auto_trigger_for_recoil), sizeof(config.b_adjust_auto_trigger_for_recoil));
        file.write(reinterpret_cast<const char*>(&config.i_auto_trigger_recoil_adjustment_amount), sizeof(config.i_auto_trigger_recoil_adjustment_amount));
        file.write(reinterpret_cast<const char*>(&config.i_auto_trigger_recoil_adjustment_wait_time), sizeof(config.i_auto_trigger_recoil_adjustment_wait_time));
        file.write(reinterpret_cast<const char*>(&config.i_auto_trigger_delay), sizeof(config.i_auto_trigger_delay));
        file.write(reinterpret_cast<const char*>(&config.i_sensitivity), sizeof(config.i_sensitivity));
        file.write(reinterpret_cast<const char*>(&config.b_predict_next_frame), sizeof(config.b_predict_next_frame));
        file.write(reinterpret_cast<const char*>(&config.i_frames_needed_for_prediction), sizeof(config.i_frames_needed_for_prediction));
        file.write(reinterpret_cast<const char*>(&config.i_max_prediction_frames), sizeof(config.i_max_prediction_frames));
        file.write(reinterpret_cast<const char*>(&config.i_future_frames_to_predict), sizeof(config.i_future_frames_to_predict));
        file.write(reinterpret_cast<const char*>(&config.e_target_mode), sizeof(config.e_target_mode));
        file.write(reinterpret_cast<const char*>(&config.e_aim_position), sizeof(config.e_aim_position));
        file.write(reinterpret_cast<const char*>(&config.b_anti_target_jitter), sizeof(config.b_anti_target_jitter));
        file.write(reinterpret_cast<const char*>(&config.d_maximum_jitter_amount), sizeof(config.d_maximum_jitter_amount));
        file.write(reinterpret_cast<const char*>(&config.i_maximum_wait_time_for_target_reappearence), sizeof(config.i_maximum_wait_time_for_target_reappearence));
        file.write(reinterpret_cast<const char*>(&config.b_save_training_data), sizeof(config.b_save_training_data));
        file.write(reinterpret_cast<const char*>(&config.b_only_run_on_hidden_cursor), sizeof(config.b_only_run_on_hidden_cursor));
        file.write(reinterpret_cast<const char*>(&config.i_fps_cap), sizeof(config.i_fps_cap));
        file.write(reinterpret_cast<const char*>(&config.b_limit_fps), sizeof(config.b_limit_fps));
        file.write(reinterpret_cast<const char*>(&config.i_selected_model_index), sizeof(config.i_selected_model_index));
        file.write(reinterpret_cast<const char*>(&config.f_minimum_top_margin), sizeof(config.f_minimum_top_margin));
        file.write(reinterpret_cast<const char*>(&config.i_minimum_confidence), sizeof(config.i_minimum_confidence));
        file.write(reinterpret_cast<const char*>(&config.b_geforce_now_mode), sizeof(config.b_geforce_now_mode));
        file.write(reinterpret_cast<const char*>(&config.b_counter_strafe), sizeof(config.b_counter_strafe));
        file.write(reinterpret_cast<const char*>(&config.b_draw_targets_on_screen), sizeof(config.b_draw_targets_on_screen));
        file.write(reinterpret_cast<const char*>(&config.b_draw_confidence), sizeof(config.b_draw_confidence));
        file.write(reinterpret_cast<const char*>(&config.i_head_margin), sizeof(config.i_head_margin));
        file.write(reinterpret_cast<const char*>(&config.b_aim_fov), sizeof(config.b_aim_fov));
        file.write(reinterpret_cast<const char*>(&config.b_draw_aim_fov), sizeof(config.b_draw_aim_fov));
        file.write(reinterpret_cast<const char*>(&config.i_fov_radius), sizeof(config.i_fov_radius));
        file.write(reinterpret_cast<const char*>(&config.c_fov), sizeof(config.c_fov));
        file.write(reinterpret_cast<const char*>(&config.c_esp), sizeof(config.c_esp));
        file.write(reinterpret_cast<const char*>(&config.b_int8_mode), sizeof(config.b_int8_mode));
        file.write(reinterpret_cast<const char*>(&config.b_use_new_gui), sizeof(config.b_use_new_gui));
        file.write(reinterpret_cast<const char*>(&config.b_performance_mode), sizeof(config.b_performance_mode));
        file.write(reinterpret_cast<const char*>(&config.read_only__i_fps), sizeof(config.read_only__i_fps));


        if (file.fail()) {
            std::cerr << XorStr("Error writing to file: ") << filePath << std::endl;
            file.close();
            return false;
        }

        file.close();
        return true;
    }

    bool deserialize_aim_config(AimConfig& config, const std::string& filePath) {
        std::ifstream file(filePath, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << XorStr("Error opening file for reading: ") << filePath << std::endl;
            return false;
        }
        file.read(reinterpret_cast<char*>(&config.config_iteration), sizeof(config.config_iteration));
        file.read(reinterpret_cast<char*>(&config.b_aimbot), sizeof(config.b_aimbot));
        file.read(reinterpret_cast<char*>(&config.b_always_aim), sizeof(config.b_always_aim));
        file.read(reinterpret_cast<char*>(&config.k_aim_key), sizeof(config.k_aim_key));
        file.read(reinterpret_cast<char*>(&config.k_triggerbot_key), sizeof(config.k_triggerbot_key));
        file.read(reinterpret_cast<char*>(&config.b_auto_trigger), sizeof(config.b_auto_trigger));
        file.read(reinterpret_cast<char*>(&config.b_adjust_auto_trigger_for_recoil), sizeof(config.b_adjust_auto_trigger_for_recoil));
        file.read(reinterpret_cast<char*>(&config.i_auto_trigger_recoil_adjustment_amount), sizeof(config.i_auto_trigger_recoil_adjustment_amount));
        file.read(reinterpret_cast<char*>(&config.i_auto_trigger_recoil_adjustment_wait_time), sizeof(config.i_auto_trigger_recoil_adjustment_wait_time));
        file.read(reinterpret_cast<char*>(&config.i_auto_trigger_delay), sizeof(config.i_auto_trigger_delay));
        file.read(reinterpret_cast<char*>(&config.i_sensitivity), sizeof(config.i_sensitivity));
        file.read(reinterpret_cast<char*>(&config.b_predict_next_frame), sizeof(config.b_predict_next_frame));
        file.read(reinterpret_cast<char*>(&config.i_frames_needed_for_prediction), sizeof(config.i_frames_needed_for_prediction));
        file.read(reinterpret_cast<char*>(&config.i_max_prediction_frames), sizeof(config.i_max_prediction_frames));
        file.read(reinterpret_cast<char*>(&config.i_future_frames_to_predict), sizeof(config.i_future_frames_to_predict));
        file.read(reinterpret_cast<char*>(&config.e_target_mode), sizeof(config.e_target_mode));
        file.read(reinterpret_cast<char*>(&config.e_aim_position), sizeof(config.e_aim_position));
        file.read(reinterpret_cast<char*>(&config.b_anti_target_jitter), sizeof(config.b_anti_target_jitter));
        file.read(reinterpret_cast<char*>(&config.d_maximum_jitter_amount), sizeof(config.d_maximum_jitter_amount));
        file.read(reinterpret_cast<char*>(&config.i_maximum_wait_time_for_target_reappearence), sizeof(config.i_maximum_wait_time_for_target_reappearence));
        file.read(reinterpret_cast<char*>(&config.b_save_training_data), sizeof(config.b_save_training_data));
        file.read(reinterpret_cast<char*>(&config.b_only_run_on_hidden_cursor), sizeof(config.b_only_run_on_hidden_cursor));
        file.read(reinterpret_cast<char*>(&config.i_fps_cap), sizeof(config.i_fps_cap));
        file.read(reinterpret_cast<char*>(&config.b_limit_fps), sizeof(config.b_limit_fps));
        file.read(reinterpret_cast<char*>(&config.i_selected_model_index), sizeof(config.i_selected_model_index));
        file.read(reinterpret_cast<char*>(&config.f_minimum_top_margin), sizeof(config.f_minimum_top_margin));
        file.read(reinterpret_cast<char*>(&config.i_minimum_confidence), sizeof(config.i_minimum_confidence));
        file.read(reinterpret_cast<char*>(&config.b_geforce_now_mode), sizeof(config.b_geforce_now_mode));
        file.read(reinterpret_cast<char*>(&config.b_counter_strafe), sizeof(config.b_counter_strafe));
        file.read(reinterpret_cast<char*>(&config.b_draw_targets_on_screen), sizeof(config.b_draw_targets_on_screen));
        file.read(reinterpret_cast<char*>(&config.b_draw_confidence), sizeof(config.b_draw_confidence));
        file.read(reinterpret_cast<char*>(&config.i_head_margin), sizeof(config.i_head_margin));
        file.read(reinterpret_cast<char*>(&config.b_aim_fov), sizeof(config.b_aim_fov));
        file.read(reinterpret_cast<char*>(&config.b_draw_aim_fov), sizeof(config.b_draw_aim_fov));
        file.read(reinterpret_cast<char*>(&config.i_fov_radius), sizeof(config.i_fov_radius));
        file.read(reinterpret_cast<char*>(&config.c_fov), sizeof(config.c_fov));
        file.read(reinterpret_cast<char*>(&config.c_esp), sizeof(config.c_esp));
        file.read(reinterpret_cast<char*>(&config.b_int8_mode), sizeof(config.b_int8_mode));
        file.read(reinterpret_cast<char*>(&config.b_use_new_gui), sizeof(config.b_use_new_gui));
        file.read(reinterpret_cast<char*>(&config.b_performance_mode), sizeof(config.b_performance_mode));
        file.read(reinterpret_cast<char*>(&config.read_only__i_fps), sizeof(config.read_only__i_fps));


        if (file.fail()) {
            std::cerr << XorStr("Error reading from file: ") << filePath << std::endl;
            file.close();
            return false;
        }
        file.close();
        return true;
    }

    void load_config_or_init_new(AimConfig* cfg) {
        namespace fs = std::filesystem;
        if (fs::exists(configFilePath)) {
            deserialize_aim_config(*cfg, configFilePath);
            if (cfg->config_iteration != CONFIG_ITERATION) {
                printf(XorStr("Unable to load config from file as it is an older version. Regenerating a new one!\n"));
                utils::init_config_with_defaults(cfg);
                serialize_aim_config(*cfg, configFilePath);
            }
        }
        else {
            utils::init_config_with_defaults(cfg);
            std::ofstream file(configFilePath);
            file.close();
            serialize_aim_config(*cfg, configFilePath);
        }
    }

    void save_config(AimConfig* cfg) {
        serialize_aim_config(*cfg, configFilePath);
    }
}