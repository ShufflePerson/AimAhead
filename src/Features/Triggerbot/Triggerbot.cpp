#include "Triggerbot.h"

namespace triggerbot {
    bool fire_weapon = false;
    bool currently_fire_active = false;

    void auto_fire_tick(bool should_fire) {
        if (!currently_fire_active && !should_fire) return;
        if (should_fire) {
            if (!currently_fire_active) {
                currently_fire_active = true;
                input::send_input_mouse_event(true);
            }
        }
        else {
            if (currently_fire_active) {
                currently_fire_active = false;
                input::send_input_mouse_event(false);
            }
        }
    }
}