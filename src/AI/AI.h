#pragma once
#include "../Helpers/logger.h"
#include "../Engine/Engine.h"
#include <chrono>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/opencv.hpp>
#include "../Capture/Capture.h"
#include "../Structs/BoundingBox.h"
#include "../Structs/CurrentTarget.h"
#include "../Utils/Utils.h"
#include "../MathHelpers/MathHelpers.h"
#include "../Input/Input.h"
#include "../Features/Triggerbot/Triggerbot.h"
#include "../Security/xor.h"

namespace ai {
	std::vector<BoundingBox> runInference(cv::cuda::GpuMat& img, float minObjectness, Engine<float>& engine);
	void main_loop(AimConfig* cfg);
}