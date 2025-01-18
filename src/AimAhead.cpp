
#include "cmd_line_parser.h"
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
#include "Structs/Config.h"
#include "Structs/CurrentTarget.h"


Options init_default_engine_options() {
    Options options;
    options.precision = Precision::FP16;
    options.calibrationDataDirectoryPath = "";
    options.optBatchSize = 1;
    options.maxBatchSize = 1;
    options.engineFileDir = ".";
    return options;
}

void init_default_engine(Engine<float>* engine) {
    std::array<float, 3> subVals{ 0.f, 0.f, 0.f };
    std::array<float, 3> divVals{ 1.f, 1.f, 1.f };
    bool normalize = true;

    spdlog::info("Loading the AI model...");
    if (!engine->buildLoadNetwork("model.onnx", subVals, divVals, normalize)) {
        throw std::runtime_error("Unable to build or load TensorRT engine.");
    }
    spdlog::info("AI Model loaded!");
}

int main(int argc, char* argv[]) {
    AimConfig cfg;
    utils::init_config_with_defaults(&cfg);
    gui::load_config(&cfg);


    if (!capture::InitializeCapture()) {
        throw std::runtime_error("Failed to start screen capture.");
        return 1;
    }

    std::thread all_threads(utils::start_threads);
    Options options = init_default_engine_options();

    Engine<float> engine(options);
    init_default_engine(&engine);

    ai::main_loop(&engine, &cfg);


    return 0;
}