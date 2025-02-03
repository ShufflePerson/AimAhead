#pragma once
#include <windows.h>
#include <corecrt_math.h>
#include <string>
#include <map>
#include <iostream>
#include <sstream> 

#include "../Structs/Config.h"
#include "../Utils/Utils.h"


namespace input {
	void send_input_mouse_relative(double dx, double dy, AimConfig *cfg);
	void send_input_mouse_event(bool down);

	bool is_mouse_hidden();

	std::string get_vk_string(int vkCode);
	int get_vk_code(const std::string& name);
	DWORD get_last_pressed_key();
}