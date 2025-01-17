#include "Collector.h"


namespace collector {
	std::vector<cv::Mat> queue = {};
	auto last_save_time = std::chrono::system_clock::now();

	bool can_save() {
		auto now_time = std::chrono::system_clock::now();
		auto elapsed_time = now_time - last_save_time;
		return elapsed_time >= std::chrono::milliseconds(500);
	}

	void save_image(cv::Mat img) {
		if (!can_save()) return;
		last_save_time = std::chrono::system_clock::now();
		queue.push_back(img);
	}

	void main_loop() {
		while (true) {
			if (queue.size() > 0) {
				for (auto it = queue.begin(); it != queue.end(); ) {
					std::string file_name = "./data/" + utils::generate_uuidv4() + ".jpg";
					cv::imwrite(file_name, *it);
					it = queue.erase(it);
				}
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		}
	}
}