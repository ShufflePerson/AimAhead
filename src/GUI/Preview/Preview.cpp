#include "Preview.h" 

namespace preview {
	bool b_loaded_cached_image = false;
	cv::Mat cached_image;
    cv::Mat rendered_image;

	void load_preview_image_into_cache() {
		b_loaded_cached_image = true;
		std::string imagePath = "./bin/preview.jpg";
		cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);
		cached_image = image;
	}

	cv::Mat get_cached_preview_image() {
		if (!b_loaded_cached_image) {
			load_preview_image_into_cache();
		}

		return cached_image;
	}

	cv::Mat get_cached_rendered_image() {
		return rendered_image;
	}

    void set_rendered_image(cv::Mat& img) {
        img.copyTo(rendered_image);
    }

}