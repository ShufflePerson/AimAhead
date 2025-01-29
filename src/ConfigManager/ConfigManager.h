#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>
#include <filesystem>
#include "../Utils/Utils.h"
#include "../Structs/Config.h"

namespace config_manager {
	bool serialize_aim_config(const AimConfig& config, const std::string& filePath);
	bool deserialize_aim_config(AimConfig& config, const std::string& filePath);

	void load_config_or_init_new(AimConfig* cfg);
	void save_config(AimConfig* cfg);
}