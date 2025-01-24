#include "Utils.h"



namespace utils {

    bool fire_weapon = false;
    bool currently_fire_active = false;

    void auto_fire_tick(bool should_fire) {
        if (!currently_fire_active && !should_fire) return;
        INPUT input[2];
        input[0].type = INPUT_MOUSE;
        input[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
        input[0].mi.dx = 0;
        input[0].mi.dy = 0;
        input[0].mi.mouseData = 0;
        input[0].mi.dwExtraInfo = 0;
        input[0].mi.time = 0;

        input[1].type = INPUT_MOUSE;
        input[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
        input[1].mi.dx = 0;
        input[1].mi.dy = 0;
        input[1].mi.mouseData = 0;
        input[1].mi.dwExtraInfo = 0;
        input[1].mi.time = 0;
        if (should_fire) {
            if (!currently_fire_active) {
                currently_fire_active = true;
                SendInput(1, &input[0], sizeof(INPUT));
            }
        }
        else {
            if (currently_fire_active) {
                currently_fire_active = false;
                SendInput(1, &input[1], sizeof(INPUT));
            }
        }
    }

    std::pair<double, double> calculateMouseMovement(int xmin, int xmax, int ymin, int ymax, double sensitivity, double startX, double startY, EAimPosition aim_position) {
        double targetX = (xmin + xmax) / 2.0;
        double targetY = (ymin + ymax) / 2.0;

        double deltaX_raw = targetX - startX;
        double deltaY_raw = targetY - startY;
        double distance = std::sqrt(std::pow(targetX - startX, 2) + std::pow(targetY - startY, 2));


        if (aim_position == TOP) {
            double boxHeight = abs(ymax - ymin);
            double margin_from_top = boxHeight * 0.20;
            if (margin_from_top <= 15.0f) {
                margin_from_top = 15.0f;
            }
            deltaY_raw = (ymin + margin_from_top) - startY;
        }

        if (aim_position == CHEST) {
            double boxHeight = abs(ymax - ymin);
            double margin_from_top = boxHeight * 0.35;
            if (margin_from_top <= 15.0f) {
                margin_from_top = 15.0f;
            }
            deltaY_raw = (ymin + margin_from_top) - startY;
        }


        return std::make_pair(deltaX_raw * sensitivity, deltaY_raw * sensitivity);
    }

    std::pair<double, double> calculateNewFramePredictionBasedoOnHistory(std::vector<std::pair<double, double>> history, std::pair<double, double> current) {
        if (history.size() < 2) {
            return current;
        }

        double weightedDeltaXSum = 0.0;
        double weightedDeltaYSum = 0.0;
        double totalWeight = 0.0;
        int total_size = history.size();

        for (int i = 1; i < total_size; i++) {
            auto currentHistory = history[i];
            auto previousHistory = history[i - 1];
            double deltaX = currentHistory.first - previousHistory.first;
            double deltaY = currentHistory.second - previousHistory.second;

            double weight = (double)(total_size - i) / (total_size - 1);
            weightedDeltaXSum += deltaX * weight;
            weightedDeltaYSum += deltaY * weight;
            totalWeight += weight;
        }

        double deltaX = weightedDeltaXSum / totalWeight;
        double deltaY = weightedDeltaYSum / totalWeight;

        double predictedX = current.first + deltaX;
        double predictedY = current.second + deltaY;

        return std::pair<double, double>{predictedX, predictedY};
    }

    double getDeltaBetweenPositions(std::pair<double, double> old_pos, std::pair<double, double> new_pos) {
        double deltaX = abs(old_pos.first - new_pos.first);
        double deltaY = abs(old_pos.second - new_pos.second);

        double result = deltaX + deltaY;
        return result;
    }

    void moveMouseRelative(double dx, double dy) {
        static double accumulated_dx = 0.0;
        static double accumulated_dy = 0.0;

        accumulated_dx += dx;
        accumulated_dy += dy;

        int move_x = static_cast<int>(floor(accumulated_dx));
        int move_y = static_cast<int>(floor(accumulated_dy));

        if (move_x != 0 || move_y != 0) {
            INPUT input = {};
            input.type = INPUT_MOUSE;
            input.mi.dwFlags = MOUSEEVENTF_MOVE;
            input.mi.dx = move_x;
            input.mi.dy = move_y;
            SendInput(1, &input, sizeof(INPUT));

            accumulated_dx -= move_x;
            accumulated_dy -= move_y;
        }

    }

    float squared_distance(float x1, float y1, float x2, float y2) {
        return std::pow(x1 - x2, 2) + std::pow(y1 - y2, 2);
    }

    std::pair<float, float> get_center(const BoundingBox& box) {
        float center_x = (box.xmin + box.xmax) / 2.0f;
        float center_y = (box.ymin + box.ymax) / 2.0f;
        return { center_x, center_y };
    }

    BoundingBox findClosest(std::vector<BoundingBox>& boxes) {
        float target_x = 320.0f;
        float target_y = 320.0f;
        float min_distance = 99999;
        BoundingBox closest_box = boxes[0];

        for (const auto& box : boxes) {
            std::pair<float, float> center = get_center(box);
            float dist = squared_distance(center.first, center.second, target_x, target_y);

            if (dist < min_distance) {
                min_distance = dist;
                closest_box = box;
            }
        }

        return closest_box;

    }

    HWND getHwndFromTitle(const std::string windowTitle) {
        HWND hwnd = FindWindow(nullptr, windowTitle.c_str());
        return hwnd;
    }

    bool IsMouseCursorHidden() {
        CURSORINFO cursorInfo = { sizeof(CURSORINFO) };
        if (GetCursorInfo(&cursorInfo)) {
            return !(cursorInfo.flags & CURSOR_SHOWING);
        }
        return false;
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
        cfg->b_auto_trigger = true;
        cfg->b_predict_next_frame = false;
        cfg->b_anti_target_jitter = true;
        cfg->b_save_training_data = true;
        cfg->b_aimbot = true;
        cfg->b_draw_targets_on_screen = true;
        cfg->b_only_run_on_hidden_cursor = false;
        cfg->b_always_aim = false;
        cfg->e_target_mode = CLOSEST;
        cfg->e_aim_position = CHEST;
        cfg->i_frames_needed_for_prediction = 4;
        cfg->i_max_prediction_frames = 10;
        cfg->d_maximum_jitter_amount = 30.0f;
        cfg->i_maximum_wait_time_for_target_reappearence = 40;
        cfg->i_sensitivity = 0.22;
        cfg->i_auto_trigger_delay = 0;
        cfg->i_fps_cap = 120;
        cfg->read_only__i_fps = 0;
    }

    void start_threads() {
        std::thread init_debug(debug::init);
        std::thread init_screen(screen::run);
        std::thread screen_loop(screen::main_loop);
        std::thread collector_loop(collector::main_loop);
        std::thread gui_loop(gui::init_gui);
        spdlog::info("Started debug::init, screen::run, screen::main_loop, collector::main_loop, gui::init_gui threads.");
        init_debug.join();
        init_screen.join();
        screen_loop.join();
        collector_loop.join();
        gui_loop.join();
    }

    // Helper function to convert VK code to string
    std::string getVKString(int vkCode) {
        static std::map<int, std::string> vkNameMap = {
            {VK_LBUTTON, "Left Mouse Button"},
            {VK_RBUTTON, "Right Mouse Button"},
            {VK_MBUTTON, "Middle Mouse Button"},
            {VK_XBUTTON1, "Mouse X1 Button"},
            {VK_XBUTTON2, "Mouse X2 Button"},
            {VK_BACK, "Backspace"},
            {VK_TAB, "Tab"},
            {VK_CLEAR, "Clear"},
            {VK_RETURN, "Enter"},
            {VK_SHIFT, "Shift"},
            {VK_CONTROL, "Control"},
            {VK_MENU, "Alt"},
            {VK_PAUSE, "Pause"},
            {VK_CAPITAL, "Caps Lock"},
            {VK_ESCAPE, "Escape"},
            {VK_SPACE, "Spacebar"},
            {VK_PRIOR, "Page Up"},
            {VK_NEXT, "Page Down"},
            {VK_END, "End"},
            {VK_HOME, "Home"},
            {VK_LEFT, "Left Arrow"},
            {VK_UP, "Up Arrow"},
            {VK_RIGHT, "Right Arrow"},
            {VK_DOWN, "Down Arrow"},
            {VK_SELECT, "Select"},
            {VK_PRINT, "Print"},
            {VK_EXECUTE, "Execute"},
            {VK_SNAPSHOT, "Print Screen"},
            {VK_INSERT, "Insert"},
            {VK_DELETE, "Delete"},
            {VK_HELP, "Help"},
            {0x30, "0"}, {0x31, "1"}, {0x32, "2"}, {0x33, "3"}, {0x34, "4"},
            {0x35, "5"}, {0x36, "6"}, {0x37, "7"}, {0x38, "8"}, {0x39, "9"},
            {0x41, "A"}, {0x42, "B"}, {0x43, "C"}, {0x44, "D"}, {0x45, "E"},
            {0x46, "F"}, {0x47, "G"}, {0x48, "H"}, {0x49, "I"}, {0x4A, "J"},
            {0x4B, "K"}, {0x4C, "L"}, {0x4D, "M"}, {0x4E, "N"}, {0x4F, "O"},
            {0x50, "P"}, {0x51, "Q"}, {0x52, "R"}, {0x53, "S"}, {0x54, "T"},
            {0x55, "U"}, {0x56, "V"}, {0x57, "W"}, {0x58, "X"}, {0x59, "Y"},
            {0x5A, "Z"},
            {VK_NUMPAD0, "Numpad 0"}, {VK_NUMPAD1, "Numpad 1"},
            {VK_NUMPAD2, "Numpad 2"}, {VK_NUMPAD3, "Numpad 3"}, {VK_NUMPAD4, "Numpad 4"},
            {VK_NUMPAD5, "Numpad 5"}, {VK_NUMPAD6, "Numpad 6"}, {VK_NUMPAD7, "Numpad 7"},
            {VK_NUMPAD8, "Numpad 8"}, {VK_NUMPAD9, "Numpad 9"},
            {VK_MULTIPLY, "Numpad *"}, {VK_ADD, "Numpad +"},
            {VK_SUBTRACT, "Numpad -"}, {VK_DECIMAL, "Numpad ."}, {VK_DIVIDE, "Numpad /"},
            {VK_F1, "F1"}, {VK_F2, "F2"}, {VK_F3, "F3"},
            {VK_F4, "F4"}, {VK_F5, "F5"}, {VK_F6, "F6"}, {VK_F7, "F7"},
            {VK_F8, "F8"}, {VK_F9, "F9"}, {VK_F10, "F10"}, {VK_F11, "F11"}, {VK_F12, "F12"},
            {VK_LSHIFT, "Left Shift"},
            {VK_RSHIFT, "Right Shift"},
            {VK_LCONTROL, "Left Control"},
            {VK_RCONTROL, "Right Control"},
            {VK_LMENU, "Left Alt"},
            {VK_RMENU, "Right Alt"},
            {VK_LWIN, "Left Windows"},
            {VK_RWIN, "Right Windows"},
        };

        if (vkNameMap.count(vkCode)) {
            return vkNameMap[vkCode];
        }

        std::stringstream ss;
        ss << "Unknown Key: " << vkCode;

        return ss.str();
    }

    int getVKCode(const std::string& name) {
        static std::map<std::string, int> vkNameMap = {
            {"Left Mouse Button", VK_LBUTTON},
            {"Right Mouse Button", VK_RBUTTON},
            {"Middle Mouse Button", VK_MBUTTON},
            {"Mouse X1 Button", VK_XBUTTON1},
            {"Mouse X2 Button", VK_XBUTTON2},
            {"Backspace", VK_BACK},
            {"Tab", VK_TAB},
            {"Clear", VK_CLEAR},
            {"Enter", VK_RETURN},
            {"Shift", VK_SHIFT},
            {"Control", VK_CONTROL},
            {"Alt", VK_MENU},
            {"Pause", VK_PAUSE},
            {"Caps Lock", VK_CAPITAL},
            {"Escape", VK_ESCAPE},
            {"Spacebar", VK_SPACE},
            {"Page Up", VK_PRIOR},
            {"Page Down", VK_NEXT},
            {"End", VK_END},
            {"Home", VK_HOME},
            {"Left Arrow", VK_LEFT},
            {"Up Arrow", VK_UP},
            {"Right Arrow", VK_RIGHT},
            {"Down Arrow", VK_DOWN},
            {"Select", VK_SELECT},
            {"Print", VK_PRINT},
            {"Execute", VK_EXECUTE},
            {"Print Screen", VK_SNAPSHOT},
            {"Insert", VK_INSERT},
            {"Delete", VK_DELETE},
            {"Help", VK_HELP},
            {"0", 0x30}, {"1", 0x31}, {"2", 0x32}, {"3", 0x33}, {"4", 0x34},
            {"5", 0x35}, {"6", 0x36}, {"7", 0x37}, {"8", 0x38}, {"9", 0x39},
            {"A", 0x41}, {"B", 0x42}, {"C", 0x43}, {"D", 0x44}, {"E", 0x45},
            {"F", 0x46}, {"G", 0x47}, {"H", 0x48}, {"I", 0x49}, {"J", 0x4A},
            {"K", 0x4B}, {"L", 0x4C}, {"M", 0x4D}, {"N", 0x4E}, {"O", 0x4F},
            {"P", 0x50}, {"Q", 0x51}, {"R", 0x52}, {"S", 0x53}, {"T", 0x54},
            {"U", 0x55}, {"V", 0x56}, {"W", 0x57}, {"X", 0x58}, {"Y", 0x59},
            {"Z", 0x5A},
            {"Numpad 0", VK_NUMPAD0}, {"Numpad 1", VK_NUMPAD1},
            {"Numpad 2", VK_NUMPAD2}, {"Numpad 3", VK_NUMPAD3}, {"Numpad 4", VK_NUMPAD4},
            {"Numpad 5", VK_NUMPAD5}, {"Numpad 6", VK_NUMPAD6}, {"Numpad 7", VK_NUMPAD7},
            {"Numpad 8", VK_NUMPAD8}, {"Numpad 9", VK_NUMPAD9},
            {"Numpad *", VK_MULTIPLY}, {"Numpad +", VK_ADD},
            {"Numpad -", VK_SUBTRACT}, {"Numpad .", VK_DECIMAL}, {"Numpad /", VK_DIVIDE},
            {"F1", VK_F1}, {"F2", VK_F2}, {"F3", VK_F3},
            {"F4", VK_F4}, {"F5", VK_F5}, {"F6", VK_F6}, {"F7", VK_F7},
            {"F8", VK_F8}, {"F9", VK_F9}, {"F10", VK_F10}, {"F11", VK_F11}, {"F12", VK_F12},
            {"Left Shift", VK_LSHIFT},
            {"Right Shift", VK_RSHIFT},
            {"Left Control", VK_LCONTROL},
            {"Right Control", VK_RCONTROL},
            {"Left Alt", VK_LMENU},
            {"Right Alt", VK_RMENU},
            {"Left Windows", VK_LWIN},
            {"Right Windows", VK_RWIN},
        };

        if (vkNameMap.count(name)) {
            return vkNameMap[name];
        }

        return -1; 
    }

}