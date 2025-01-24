#include "Input.h"

namespace input {
	void send_input_mouse_relative(double dx, double dy) {
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

    void send_input_mouse_event(bool down) {
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
        if (down) {
            SendInput(1, &input[0], sizeof(INPUT));
        }
        else {
            SendInput(1, &input[1], sizeof(INPUT));
        }
    }

    bool is_mouse_hidden() {
        CURSORINFO cursorInfo = { sizeof(CURSORINFO) };
        if (GetCursorInfo(&cursorInfo)) {
            return !(cursorInfo.flags & CURSOR_SHOWING);
        }
        return false;
    }
}