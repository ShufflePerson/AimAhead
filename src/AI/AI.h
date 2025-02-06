#pragma once
#include "../Helpers/logger.h"
#include "../Engine/Engine.h"
#include <chrono>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/opencv.hpp>
#include "../Capture/Capture.h"
#include "../Structs/BoundingBox.h"
#include "../Structs/CurrentTarget.h"
#include "../Structs/DebugInfo.h"
#include "../Structs/Vector2.h"
#include "../Utils/Utils.h"
#include "../MathHelpers/MathHelpers.h"
#include "../Input/Input.h"
#include "../Features/Triggerbot/Triggerbot.h"
#include "../Security/xor.h"
#include "../Security/Security.h"
#include "../Connection/Connection.h"

namespace ai {
	std::vector<BoundingBox> runInference(cv::cuda::GpuMat& img, float minObjectness, Engine<float>& engine);
	void main_loop(AimConfig* cfg);
	void init_default_engine(Engine<float>* engine, AimConfig* config);
	Options init_default_engine_options();
	
	void handle_model_changes(AimConfig* cfg, Engine<float>* engine);
	void handle_preview(AimConfig* cfg, Engine<float>& engine_ptr);
	void handle_save_training_data(AimConfig* cfg, cv::cuda::GpuMat& gpuImg);
	BoundingBox calculate_target_box(std::vector<BoundingBox> results, AimConfig* cfg);
}