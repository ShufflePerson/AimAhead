#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <filesystem>

#include "../Utils/Utils.h"
#include "../Security/xor.h"

namespace model_manager {
	void update_models_list();
	std::vector<std::string> get_loaded_models();
	std::vector<std::string> get_all_models();
}