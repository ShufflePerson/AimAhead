#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <mutex>
#include "../Structs/BoundingBox.h"
#include "../Utils/Utils.h"

//#define __DEBUG__ 1
//#define __BENCHMARK__ 1

namespace debug {
	void init();
	void update_image(cv::Mat cpuMat);
	void draw_boxes(std::vector<BoundingBox>& boxes);
	void draw_target_location(std::pair<double, double> cords);
	void draw_target_prediction(std::pair<double, double> oldCords, std::pair<double, double> newCords);
}