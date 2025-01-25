#include "./Helpers/logger.h"
#include "./Engine/Engine.h"
#include <chrono>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/opencv.hpp>
#include "./Capture/Capture.h"
#include "./Structs/BoundingBox.h"
#include "./AI/AI.h"
#include "./Utils/Utils.h"
#include "./Debug/Debug.h"
#include "./Screen/Screen.h"
#include "./Collector/Collector.h"
#include "./GUI/GUI.h"
#include "./ModelManager/ModelManager.h"
#include "Structs/Config.h"
#include "Structs/CurrentTarget.h"



int main(int argc, char* argv[]) {
    AimConfig cfg;
    model_manager::update_models_list();
    utils::init_config_with_defaults(&cfg);
    gui::load_config(&cfg);


    if (!capture::InitializeCapture()) {
        throw std::runtime_error("Failed to start screen capture.");
        return 1;
    }

    std::thread all_threads(utils::start_threads);


    ai::main_loop(&cfg);


    return 0;
}