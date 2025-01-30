#include "Utils.h"



namespace utils {

    HWND getHwndFromTitle(const std::string windowTitle) {
        HWND hwnd = FindWindow(nullptr, windowTitle.c_str());
        return hwnd;
    }
    
    std::string generate_uuidv4() {
        // Use a strong random number generator
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 15); // Generate hex digits (0-F)
        std::uniform_int_distribution<> prob(0, 3); // For variant bits

        std::stringstream ss;
        int i;

        // Generate 16 random bytes (128 bits)
        std::array<unsigned char, 16> uuid_bytes;
        for (i = 0; i < 16; ++i) {
            uuid_bytes[i] = static_cast<unsigned char>(dis(gen) << 4 | dis(gen));
        }

        // Set the version bits (0100 for version 4)
        uuid_bytes[6] &= 0x0F; // Clear the upper 4 bits of the 7th byte
        uuid_bytes[6] |= 0x40; // Set the 4 most significant bits to 0100

        // Set the variant bits (10xx for variant specified in RFC 4122)
        uuid_bytes[8] &= 0x3F; // Clear the upper 2 bits of the 9th byte
        uuid_bytes[8] |= 0x80; // Set the 2 most significant bits to 10

        // Format the UUID as a string
        for (i = 0; i < 16; ++i) {
            if (i == 4 || i == 6 || i == 8 || i == 10) {
                ss << "-";
            }
            ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(uuid_bytes[i]);
        }

        return ss.str();
    }

    void init_config_with_defaults(AimConfig* cfg) {
        cfg->config_iteration = CONFIG_ITERATION;
        cfg->b_auto_trigger = true;
        cfg->b_predict_next_frame = false;
        cfg->b_anti_target_jitter = true;
        cfg->b_save_training_data = false;
        cfg->b_aimbot = true;
        cfg->b_draw_targets_on_screen = true;
        cfg->b_only_run_on_hidden_cursor = false;
        cfg->b_always_aim = false;
        cfg->b_adjust_auto_trigger_for_recoil = true;
        cfg->b_draw_confidence;
        cfg->e_target_mode = ETargetMode::CLOSEST;
        cfg->e_aim_position = EAimPosition::TOP;
        cfg->i_frames_needed_for_prediction = 17;
        cfg->i_max_prediction_frames = 32;
        cfg->i_future_frames_to_predict = 2;
        cfg->d_maximum_jitter_amount = 30.0f;
        cfg->f_minimum_top_margin = 15.0f;
        cfg->i_head_margin = 20;
        cfg->i_maximum_wait_time_for_target_reappearence = 40;
        cfg->i_sensitivity = 0.22;
        cfg->i_auto_trigger_delay = 0;
        cfg->i_auto_trigger_recoil_adjustment_amount = 150;
        cfg->i_auto_trigger_recoil_adjustment_wait_time = 500;
        cfg->i_fps_cap = 120;
        cfg->i_selected_model_index = 0;
        cfg->i_minimum_confidence = 55;
        cfg->read_only__i_fps = 0;
    }

    void start_threads() {
        std::thread gui_loop(gui::init_gui);
        std::thread init_debug(debug::init);
        std::thread collector_loop(collector::main_loop);
        spdlog::info("Started debug::init, collector::main_loop, gui::init_gui threads.");
        init_debug.join();
        collector_loop.join();
        gui_loop.join();
    }


    long long get_current_unix_time_s() {
        auto now = std::chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
        return seconds.count();
    }

    bool write_to_file(const std::string& filename, const std::string& contents) {
        std::ofstream output_file(filename);

        if (!output_file.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;

            std::error_code error_code;
            output_file.open(filename, std::ios::out);
            if (error_code) {
                std::cerr << "System error: " << error_code.message() << " (Error code: " << error_code.value() << ", Category: " << error_code.category().name() << ")" << std::endl;
            }
            return false; 
        }

        output_file << contents; 

        if (output_file.fail()) {
            std::cerr << "Error writing to file: " << filename << std::endl;
            std::error_code error_code;
            output_file.clear(); 
            output_file.setstate(std::ios::failbit); 
            error_code = std::make_error_code(std::errc::io_error); 
            std::cerr << "System error: " << error_code.message() << " (Error code: " << error_code.value() << ", Category: " << error_code.category().name() << ")" << std::endl;
            output_file.close();
            return false; 
        }

        output_file.close();
        return true;
    }

    std::pair<int, int> get_primary_display_size() {
        int width = GetSystemMetrics(SM_CXSCREEN);
        int height = GetSystemMetrics(SM_CYSCREEN);
        return { width, height };
    }
}