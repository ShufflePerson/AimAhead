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
	std::pair<double, double> calculateMouseMovement(int xmin, int xmax, int ymin, int ymax, double sensitivity, double startX = 640 / 2.0, double startY = 640 / 2.0, EAimPosition aim_position = MIDDLE);
	std::pair<double, double> calculateNewFramePrediction(std::pair<double, double> oldPosition, std::pair<double, double> newPosition);
	std::pair<double, double> calculateNewFramePredictionBasedoOnHistory(std::vector<std::pair<double, double>> history, std::pair<double, double> current);
	double getDeltaBetweenPositions(std::pair<double, double> old_pos, std::pair<double, double> new_pos);
	void moveMouseRelative(double dx, double dy);
	void auto_fire_tick(bool should_fire);
	BoundingBox findClosest(std::vector<BoundingBox>& boxes);
	HWND getHwndFromTitle(const std::string windowTitle);
	bool IsMouseCursorHidden();
	std::string generate_uuidv4();
	void init_config_with_defaults(AimConfig* cfg);
	void start_threads();
}