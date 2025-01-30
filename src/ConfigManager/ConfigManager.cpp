#include "ConfigManager.h"


namespace config_manager {
    const std::string configFilePath = "config.cfg";

    bool serialize_aim_config(const AimConfig& config, const std::string& filePath) {
        std::ofstream file(filePath, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Error opening file for writing: " << filePath << std::endl;
            return false;
        }

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
        file.write(reinterpret_cast<const char*>(&config.e_target_mode), sizeof(config.e_target_mode));
        file.write(reinterpret_cast<const char*>(&config.e_aim_position), sizeof(config.e_aim_position));
        file.write(reinterpret_cast<const char*>(&config.b_anti_target_jitter), sizeof(config.b_anti_target_jitter));
        file.write(reinterpret_cast<const char*>(&config.d_maximum_jitter_amount), sizeof(config.d_maximum_jitter_amount));
        file.write(reinterpret_cast<const char*>(&config.i_maximum_wait_time_for_target_reappearence), sizeof(config.i_maximum_wait_time_for_target_reappearence));
        file.write(reinterpret_cast<const char*>(&config.b_save_training_data), sizeof(config.b_save_training_data));
        file.write(reinterpret_cast<const char*>(&config.b_only_run_on_hidden_cursor), sizeof(config.b_only_run_on_hidden_cursor));
        file.write(reinterpret_cast<const char*>(&config.i_fps_cap), sizeof(config.i_fps_cap));
        file.write(reinterpret_cast<const char*>(&config.i_selected_model_index), sizeof(config.i_selected_model_index));
        file.write(reinterpret_cast<const char*>(&config.f_minimum_top_margin), sizeof(config.f_minimum_top_margin));
        file.write(reinterpret_cast<const char*>(&config.i_minimum_confidence), sizeof(config.i_minimum_confidence));
        file.write(reinterpret_cast<const char*>(&config.b_draw_targets_on_screen), sizeof(config.b_draw_targets_on_screen));
        file.write(reinterpret_cast<const char*>(&config.b_draw_confidence), sizeof(config.b_draw_confidence));
        file.write(reinterpret_cast<const char*>(&config.i_head_margin), sizeof(config.i_head_margin));
        file.write(reinterpret_cast<const char*>(&config.read_only__i_fps), sizeof(config.read_only__i_fps));


        if (file.fail()) {
            std::cerr << "Error writing to file: " << filePath << std::endl;
            file.close();
            return false;
        }

        file.close();
        return true;
    }

    bool deserialize_aim_config(AimConfig& config, const std::string& filePath) {
        std::ifstream file(filePath, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Error opening file for reading: " << filePath << std::endl;
            return false;
        }

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
        file.read(reinterpret_cast<char*>(&config.e_target_mode), sizeof(config.e_target_mode));
        file.read(reinterpret_cast<char*>(&config.e_aim_position), sizeof(config.e_aim_position));
        file.read(reinterpret_cast<char*>(&config.b_anti_target_jitter), sizeof(config.b_anti_target_jitter));
        file.read(reinterpret_cast<char*>(&config.d_maximum_jitter_amount), sizeof(config.d_maximum_jitter_amount));
        file.read(reinterpret_cast<char*>(&config.i_maximum_wait_time_for_target_reappearence), sizeof(config.i_maximum_wait_time_for_target_reappearence));
        file.read(reinterpret_cast<char*>(&config.b_save_training_data), sizeof(config.b_save_training_data));
        file.read(reinterpret_cast<char*>(&config.b_only_run_on_hidden_cursor), sizeof(config.b_only_run_on_hidden_cursor));
        file.read(reinterpret_cast<char*>(&config.i_fps_cap), sizeof(config.i_fps_cap));
        file.read(reinterpret_cast<char*>(&config.i_selected_model_index), sizeof(config.i_selected_model_index));
        file.read(reinterpret_cast<char*>(&config.f_minimum_top_margin), sizeof(config.f_minimum_top_margin));
        file.read(reinterpret_cast<char*>(&config.i_minimum_confidence), sizeof(config.i_minimum_confidence));
        file.read(reinterpret_cast<char*>(&config.b_draw_targets_on_screen), sizeof(config.b_draw_targets_on_screen));
        file.read(reinterpret_cast<char*>(&config.b_draw_confidence), sizeof(config.b_draw_confidence));
        file.read(reinterpret_cast<char*>(&config.i_head_margin), sizeof(config.i_head_margin));
        file.read(reinterpret_cast<char*>(&config.read_only__i_fps), sizeof(config.read_only__i_fps));


        if (file.fail()) {
            std::cerr << "Error reading from file: " << filePath << std::endl;
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