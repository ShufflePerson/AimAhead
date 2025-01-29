#pragma once
#include <chrono>
#include "../../Input/Input.h"
#include "../../Structs/Config.h"

namespace triggerbot {
	void auto_fire_tick(bool should_fire, double& y_sens_multiplier, AimConfig* cfg);
}