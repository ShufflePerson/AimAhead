#include "ModelManager.h"


namespace model_manager {
	std::vector<std::string> models = std::vector<std::string>{};
	long long last_update_time = 0;
	long long time_between_model_updates_s = 5;

	void update_models_list() {
		long long current_time = utils::get_current_unix_time_s();
		if (current_time - last_update_time > time_between_model_updates_s) {
			last_update_time = current_time;
            models.clear();
            models = get_all_models();
		}
	}

    
    std::vector<std::string> get_loaded_models() {
        return models;
    }

    bool ends_with_onnx(const std::string& str) {
        std::string suffix = ".onnx";
        if (str.length() >= suffix.length()) {
            return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
        }
        else {
            return false;
        }
    }

    std::vector<std::string> get_all_models() {
        namespace fs = std::filesystem;

        std::vector<std::string> files;
        fs::path modelsDir = "./models";

        if (!fs::exists(modelsDir) || !fs::is_directory(modelsDir)) {
            std::cerr << "Error: './models' directory does not exist or is not a directory." << std::endl;
            return files;
        }

        for (const auto& entry : fs::directory_iterator(modelsDir)) {
            if (fs::is_regular_file(entry)) {
                std::string model_name = (entry.path().filename().string()); 
                if (ends_with_onnx(model_name)) {
                    files.push_back(model_name);
                }
            }
        }
        return files;
    }
}