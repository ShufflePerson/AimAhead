#pragma once
#include <chrono>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/opencv.hpp>
#include <Windows.h>
#include <limits>
#include <algorithm>
#include <random>
#include <iomanip>
#include <sstream>
#include <array>
#include <thread>
#include <spdlog/spdlog.h>
#include "../Debug/Debug.h"
#include "../Screen/Screen.h"
#include "../Collector/Collector.h"
#include "../GUI/GUI.h"
#include "../Structs/BoundingBox.h"
#include "../Structs/Config.h"



//#define __LOG_PREDICTION_ERRORS__ 1

namespace utils {
	std::string generate_uuidv4();
	std::string getVKString(int vkCode);

	void init_config_with_defaults(AimConfig* cfg);
	void start_threads();

	int getVKCode(const std::string& name);

	HWND getHwndFromTitle(const std::string windowTitle);
	DWORD get_last_pressed_key();
}