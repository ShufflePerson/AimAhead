#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include "../../Security/xor.h"



namespace preview {
	void load_preview_image_into_cache();
	cv::Mat get_cached_preview_image();
	void set_rendered_image(cv::Mat& img);
	cv::Mat get_cached_rendered_image();
}