#pragma once
#include <vector>
#include <opencv2/opencv.hpp>
#include "../Utils/Utils.h"

namespace collector {

	void main_loop();
	void save_image(cv::Mat img);
	bool can_save();
}