#include "AI.h"



namespace ai {

    std::vector<BoundingBox> runInference(cv::cuda::GpuMat& img, float minObjectness, Engine<float>& engine) {
        const auto& inputDims = engine.getInputDims();
        std::vector<std::vector<cv::cuda::GpuMat>> inputs;
        size_t batchSize = 1;

        for (const auto& inputDim : inputDims) {
            std::vector<cv::cuda::GpuMat> input;
            for (size_t j = 0; j < batchSize; ++j) {
                input.emplace_back(std::move(img));
            }
            inputs.emplace_back(std::move(input));
        }

        std::vector<std::vector<std::vector<float>>> featureVectors;
        if (!engine.runInference(inputs, featureVectors)) {
            const std::string msg = "Unable to run inference.";
            spdlog::error(msg);
            throw std::runtime_error(msg);
        }

        std::vector<BoundingBox> allBoundingBoxes;
        for (const auto& batchFeatures : featureVectors) {
            for (const auto& currentFeatureVector : batchFeatures) {
                const int dim2 = 5;
                const int dim3 = 8400;

                if (currentFeatureVector.size() != dim2 * dim3) {
                    std::cerr << "Error: Input vector size does not match the target tensor size." << std::endl;
                    continue;
                }

                for (int i = 0; i < dim3; ++i) {
                    float objectness = currentFeatureVector[4 * dim3 + i];
                    if (objectness >= minObjectness) {
                        float x_center = currentFeatureVector[0 * dim3 + i];
                        float y_center = currentFeatureVector[1 * dim3 + i];
                        float width = currentFeatureVector[2 * dim3 + i];
                        float height = currentFeatureVector[3 * dim3 + i];
                        float x_min = x_center - width / 2.0f;
                        float y_min = y_center - height / 2.0f;
                        float x_max = x_center + width / 2.0f;
                        float y_max = y_center + height / 2.0f;

                        BoundingBox result;
                        result.confidence = objectness;
                        result.xmin = x_min;
                        result.xmax = x_max;
                        result.ymin = y_min;
                        result.ymax = y_max;
                        allBoundingBoxes.push_back(result);
                    }
                }
            }
        }
        return allBoundingBoxes;
    }

    Options init_default_engine_options() {
        Options options;
        options.precision = Precision::FP16;
        options.calibrationDataDirectoryPath = "";
        options.optBatchSize = 1;
        options.maxBatchSize = 1;
        options.engineFileDir = "./models/";
        return options;
    }

    void init_default_engine(Engine<float>* engine, AimConfig* config) {
        std::array<float, 3> subVals{ 0.f, 0.f, 0.f };
        std::array<float, 3> divVals{ 1.f, 1.f, 1.f };
        bool normalize = true;

        spdlog::info("Loading the AI model...");
        std::string model_path = "./models/";
        model_path += model_manager::get_loaded_models()[config->i_selected_model_index];
        if (!engine->buildLoadNetwork(model_path, subVals, divVals, normalize)) {
            throw std::runtime_error("Unable to build or load TensorRT engine.");
        }
        spdlog::info("AI Model loaded!");
    }


    void main_loop(AimConfig* cfg) {
        int i_currently_loaded_model_index = cfg->i_selected_model_index;
        Engine<float>* engine_ptr = new Engine<float>(init_default_engine_options());
        init_default_engine(engine_ptr, cfg);

        using namespace std::chrono;

        // Data Capture
        std::vector<BYTE> capturedData;

        // Timing Information
        high_resolution_clock::time_point start_time, end_time;
        duration<double> elapsed_time;
        double total_elapsed_time = 0.0;

        // Iteration and Averaging
        int iteration_count = 0;
        int averaging_interval = 50;

        // Object Detection Parameters
        double area_middle = 640 / 2.0;
        float minObjectness = 0.65f;

        // Target Tracking
        std::pair<double, double> lastTargetPosition = { -5.0, -5.0 };
        double sens = 1.0;

        // Frame Information
        std::size_t current_frame_count = 0;

        // Current Target State
        CurrentTarget current_target;
        current_target.i_last_seen_frame = 0;
        current_target.v_location_history = std::vector<std::pair<double, double>>{};
        current_target.p_last_position = std::pair<double, double>{ 0.0, 0.0 };
        current_target.b_active = false;

        bool b_jitter_Detected = false;
        size_t i_jitter_detected_at = 0;
        

        //Cache for preview tab
        double i_last_sensitivity = 0;
        int i_last_head_margin = 0;
        float f_last_minimum_head_margin = 0.0f;

        //Triggerbot
        double d_additional_y_sens_multiplier = 1.0;

        while (true) {
            if (gui::get_is_visible()) {
                if (i_last_sensitivity == cfg->i_sensitivity && i_last_head_margin == cfg->i_head_margin && cfg->f_minimum_top_margin == f_last_minimum_head_margin) {
                    continue;
                }
                i_last_sensitivity = cfg->i_sensitivity;
                i_last_head_margin = cfg->i_head_margin;
                f_last_minimum_head_margin = cfg->f_minimum_top_margin;

                cv::cuda::GpuMat gpuImg;
                cv::Mat cpuImg = preview::get_cached_preview_image();
                gpuImg.upload(cpuImg);
                std::vector<BoundingBox> results = ai::runInference(gpuImg, minObjectness, *engine_ptr);
                if (results.size() == 0) continue;

                BoundingBox box = results[0];
                float highestConf = 0.0f;
                for (int i = 0; i < results.size(); i++) {
                    if (results[i].confidence >= highestConf) {
                        box = results[i];
                        highestConf = box.confidence;
                    }
                }

                std::pair<double, double> movementExact = math_helpers::calculate_mouse_movement(box.xmin, box.xmax, box.ymin, box.ymax, cfg->i_sensitivity, area_middle, area_middle, cfg->e_aim_position, cfg);
                int deltaX = static_cast<int>(round(movementExact.first / cfg->i_sensitivity));
                int deltaY = static_cast<int>(round(movementExact.second / cfg->i_sensitivity));
                deltaX += 320;
                deltaY += 320;
            

                cv::Mat newImg;
                cpuImg.copyTo(newImg);

                cv::Point topLeft(box.xmin, box.ymin);
                cv::Point bottomRight(box.xmax, box.ymax);
                cv::Scalar redColor(0, 0, 255);
                int thickness = 1;
                cv::rectangle(newImg, topLeft, bottomRight, redColor, thickness);


                cv::Point center(deltaX, deltaY);
                int radius = 5;
                cv::circle(newImg, center, radius, redColor, cv::FILLED);

                preview::set_rendered_image(newImg);
                continue;
            }

            start_time = high_resolution_clock::now();

            if (i_currently_loaded_model_index != cfg->i_selected_model_index) {
                i_currently_loaded_model_index = cfg->i_selected_model_index;
                delete engine_ptr; 
                engine_ptr = nullptr;
                engine_ptr = new Engine<float>(init_default_engine_options());
                init_default_engine(engine_ptr, cfg);
            }


            if (cfg->b_only_run_on_hidden_cursor && !input::is_mouse_hidden()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                continue;
            }

            bool holding_triggerbot_key = (GetAsyncKeyState(cfg->k_triggerbot_key) & 0x8000);
            bool holding_aim_key = cfg->b_always_aim || (GetAsyncKeyState(cfg->k_aim_key) & 0x8000) || (holding_triggerbot_key && cfg->b_auto_trigger);

            capturedData.clear();
            if (!holding_triggerbot_key) {
                triggerbot::auto_fire_tick(false, d_additional_y_sens_multiplier, cfg);
            }


            if (!capture::captureScreenRegion(capturedData)) {
                continue;
            }
            if (capturedData.empty()) {
                continue;
            }
            current_frame_count++;


            cv::cuda::Stream stream;
            cv::cuda::GpuMat gpuImg(640, 640, CV_8UC4);
            gpuImg.upload(cv::Mat(640, 640, CV_8UC4, const_cast<BYTE*>(capturedData.data())), stream);
            cv::cuda::cvtColor(gpuImg, gpuImg, cv::COLOR_BGRA2RGB, 0, stream);
            stream.waitForCompletion();


            std::vector<BoundingBox> results = ai::runInference(gpuImg, minObjectness, *engine_ptr);

#ifdef __DEBUG__
            cv::Mat cpuImg;
            gpuImg.download(cpuImg);
            cv::cvtColor(cpuImg, cpuImg, cv::COLOR_BGRA2RGB);
            debug::update_image(cpuImg);
            debug::draw_boxes(results);
#endif
            if (results.size() > 0) {
                BoundingBox box = results[0];

                if (cfg->e_target_mode == OBJECTIVE) {
                    float highestConf = 0.0f;
                    for (int i = 0; i < results.size(); i++) {
                        if (results[i].confidence >= highestConf) {
                            box = results[i];
                            highestConf = box.confidence;
                        }
                    }
                }

                if (cfg->e_target_mode == CLOSEST) {
                    box = math_helpers::find_closests(results);
                }

                if (cfg->b_draw_targets_on_screen) {
                    gui::update_boxes({ box });
                }


                if (cfg->b_save_training_data && collector::can_save()) {
                    cv::Mat cpuImg;
                    gpuImg.download(cpuImg);
                    cv::cvtColor(cpuImg, cpuImg, cv::COLOR_BGRA2RGB);
                    collector::save_image(cpuImg);
                }
#ifndef __DEBUG__
                if (!holding_aim_key) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    continue;
                }
#endif


                std::pair<double, double> movementExact = math_helpers::calculate_mouse_movement(box.xmin, box.xmax, box.ymin, box.ymax, cfg->i_sensitivity, area_middle, area_middle, cfg->e_aim_position, cfg);
                if (cfg->b_adjust_auto_trigger_for_recoil) {
                    movementExact.second = movementExact.second * d_additional_y_sens_multiplier;
                }
                int deltaX = static_cast<int>(round(movementExact.first));
                int deltaY = static_cast<int>(round(movementExact.second));
                std::pair<int, int> movement = { deltaX, deltaY };

                if (current_target.p_last_position.first != 0.0 && current_target.p_last_position.second != 0.0) {
                    double deltaBetweenLastAndNewTarget = math_helpers::get_delta_between_positions(current_target.p_last_position, movementExact);
                    if (deltaBetweenLastAndNewTarget >= cfg->d_maximum_jitter_amount) {
                        if (!b_jitter_Detected)
                            i_jitter_detected_at = current_frame_count;
                        b_jitter_Detected = true;
                    }
                    else {
                        b_jitter_Detected = false;
                        i_jitter_detected_at = 0;
                    }
                }


                if (b_jitter_Detected) {
                    size_t frameDelta = current_frame_count - i_jitter_detected_at;
                    if (frameDelta >= cfg->i_maximum_wait_time_for_target_reappearence) {
                        b_jitter_Detected = false;
                        i_jitter_detected_at = 0;
                    }
                    else {
                        std::this_thread::sleep_for(std::chrono::milliseconds(1));
                        continue;
                    }
                }


                if (cfg->b_aimbot && !b_jitter_Detected) {
                    current_target.p_last_position = movementExact;
                    if (cfg->b_predict_next_frame) {
                        if (!current_target.b_active) {
                            current_target.b_active = true;
                        }
                        current_target.v_location_history.push_back(movementExact);

                        if (current_target.v_location_history.size() > cfg->i_max_prediction_frames) {
                            int amount_to_remove = current_target.v_location_history.size() - cfg->i_max_prediction_frames;
                            current_target.v_location_history.erase(current_target.v_location_history.begin(), current_target.v_location_history.begin() + amount_to_remove);
                        }

                        if (current_target.v_location_history.size() >= cfg->i_frames_needed_for_prediction) {
                            std::pair<double, double> predictionOutcome = math_helpers::calculate_new_frame_prediction_based_on_history(current_target.v_location_history, movementExact);
#ifdef __DEBUG__
                            debug::draw_target_prediction(movementExact, predictionOutcome);
#endif
#ifndef __DEBUG__
                            input::send_input_mouse_relative(predictionOutcome.first, predictionOutcome.second);
#endif
                        }

                        if (!(current_target.v_location_history.size() >= cfg->i_frames_needed_for_prediction)) {
#ifndef __DEBUG__
                            input::send_input_mouse_relative(movementExact.first, movementExact.second);
#endif
                        }
                    }
                    else {
#ifndef __DEBUG__
                        input::send_input_mouse_relative(movementExact.first, movementExact.second);
#endif
                    }
                }

#ifdef __DEBUG__
                debug::draw_target_location(movementExact);
#endif


                if (cfg->b_auto_trigger && holding_triggerbot_key) {
                    if (cfg->i_auto_trigger_delay > 0) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(cfg->i_auto_trigger_delay));
                    }
                    if (abs(movement.first) <= 10) {
                        triggerbot::auto_fire_tick(true, d_additional_y_sens_multiplier, cfg);
                    }
                }
                lastTargetPosition = movementExact;
            }
            else {
                gui::update_boxes({});
                if (cfg->b_auto_trigger) {
                    triggerbot::auto_fire_tick(false, d_additional_y_sens_multiplier, cfg);
                }
                lastTargetPosition.first = -5.0;
                lastTargetPosition.second = -5.0;

                current_target.b_active = false;
                current_target.v_location_history.clear();
                current_target.p_last_position = std::pair<double, double>{ 0.0, 0.0 };
                i_jitter_detected_at = 0;
                b_jitter_Detected = false;
            }


#ifdef __DEBUG__
            cpuImg.release();
#endif
            gpuImg.release();


            if (cfg->i_fps_cap != 0) {
                averaging_interval = cfg->i_fps_cap;
                high_resolution_clock::time_point current_time = high_resolution_clock::now();
                duration<double> time_since_last_run = current_time - start_time;
                double desired_frame_time = 1.0 / cfg->i_fps_cap;

                if (time_since_last_run.count() < desired_frame_time) {
                    duration<double> sleep_duration(desired_frame_time - time_since_last_run.count());
                    auto target_time = high_resolution_clock::now() + sleep_duration;
                    while (high_resolution_clock::now() < target_time);
                }
            }

            end_time = high_resolution_clock::now();
            elapsed_time = end_time - start_time;

            total_elapsed_time += elapsed_time.count();
            iteration_count++;
            if (iteration_count % averaging_interval == 0) {
                double average_time = total_elapsed_time / averaging_interval;
                int fps = static_cast<int>(1.0 / average_time);
                cfg->read_only__i_fps = fps;
#ifdef __BENCHMARK__
                std::cout << "Avg. iteration time (" << iteration_count << " iters): "
                    << std::fixed << std::setprecision(6) << average_time << " seconds" << std::endl;
#endif
                total_elapsed_time = 0.0;
            }
        }
    }
}