#include "Triggerbot.h"

namespace triggerbot {
    bool fire_weapon = false;
    bool currently_fire_active = false;
    auto started_shooting_at = std::chrono::system_clock::now();


    void auto_fire_tick(bool should_fire, double& y_sens_multiplier, AimConfig* cfg) {
        if (currently_fire_active) {
            auto current_time = std::chrono::system_clock::now();
            auto time_diff = current_time - started_shooting_at;
            auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(time_diff);
            if (milliseconds > std::chrono::milliseconds(cfg->i_auto_trigger_recoil_adjustment_wait_time)) {
                y_sens_multiplier = static_cast<double>(cfg->i_auto_trigger_recoil_adjustment_amount) / 100.0;
            }
        }
        if (!currently_fire_active && !should_fire) return;
        if (should_fire) {
            if (!currently_fire_active) {
                currently_fire_active = true;
                input::send_input_mouse_event(true);
                started_shooting_at = std::chrono::system_clock::now();
            }
        }
        else {
            if (currently_fire_active) {
                currently_fire_active = false;
                input::send_input_mouse_event(false);
                y_sens_multiplier = 1.0;
            }
        }
    }
}