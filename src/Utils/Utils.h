#pragma once
#define NOMINMAX
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
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <system_error> 
#include "../Debug/Debug.h"
#include "../Collector/Collector.h"
#include "../GUI/GUI.h"
#include "../Structs/BoundingBox.h"
#include "../Structs/Config.h"
#include "../Security/xor.h"


//#define __LOG_PREDICTION_ERRORS__ 1

namespace utils {
	std::string generate_uuidv4();

	void init_config_with_defaults(AimConfig* cfg);
	void start_threads();

	HWND getHwndFromTitle(const std::string windowTitle);

	long long get_current_unix_time_s();

	bool write_to_file(const std::string& filename, const std::string& contents);

	std::pair<int, int> get_primary_display_size();

	HWND get_geforce_now_hwnd();

	std::vector<BoundingBox> get_individual_targets(std::vector<BoundingBox> &results);

}