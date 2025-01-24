#pragma once
#include <windows.h>
#include <corecrt_math.h>


namespace input {
	void send_input_mouse_relative(double dx, double dy);
	void send_input_mouse_event(bool down);

	bool is_mouse_hidden();
}