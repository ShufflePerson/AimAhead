#include "AI.h"



namespace ai {

    std::vector<BoundingBox> runInference(cv::cuda::GpuMat& img, float minObjectness, Engine<float>& engine) {
        const auto& inputDims = engine.getInputDims();
        std::vector<std::vector<cv::cuda::GpuMat>> inputs = { { img } };
        size_t batchSize = 1;

        std::vector<std::vector<std::vector<float>>> featureVectors;
        if (!engine.runInference(inputs, featureVectors)) {
            const std::string msg = XorStr("Unable to run inference.");
            spdlog::error(msg);
            throw std::runtime_error(msg);
        }
        std::vector<BoundingBox> allBoundingBoxes;
        for (const auto& batchFeatures : featureVectors) {
            for (const auto& currentFeatureVector : batchFeatures) {
                const int dim2 = 5;
                const int dim3 = 8400;

                if (currentFeatureVector.size() != dim2 * dim3) {
                    std::cerr << XorStr("Error: Input vector size does not match the target tensor size.") << std::endl;
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

    Options init_default_engine_options(AimConfig *cfg) {
        Options options;
        options.precision = cfg->b_int8_mode ? Precision::INT8 : Precision::FP16;
        options.calibrationDataDirectoryPath = XorStr("./bin/int8/");
        options.optBatchSize = 1;
        options.maxBatchSize = 1;
        options.engineFileDir = XorStr("./models/");
        return options;
    }

    void init_default_engine(Engine<float>* engine, AimConfig* config) {
        std::array<float, 3> subVals{ 0.f, 0.f, 0.f };
        std::array<float, 3> divVals{ 1.f, 1.f, 1.f };
        bool normalize = true;

        spdlog::info(XorStr("Loading the AI model..."));
        std::string model_path = XorStr("./models/");
        model_path += model_manager::get_loaded_models()[config->i_selected_model_index];
        if (!engine->buildLoadNetwork(model_path, subVals, divVals, normalize)) {
            throw std::runtime_error(XorStr("Unable to build or load TensorRT engine."));
        }
        spdlog::info(XorStr("AI Model loaded!"));
    }


    void main_loop(AimConfig* cfg) {
        Engine<float>* engine_ptr = new Engine<float>(init_default_engine_options(cfg));
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

        // Frame Information
        std::size_t current_frame_count = 0;
        steady_clock::time_point last_frame_time = steady_clock::now();;
        steady_clock::time_point current_frame_time = steady_clock::now();;
        double dt = 0.0;

        // Current Target State
        CurrentTarget current_target;
        current_target.i_last_seen_frame = 0;
        current_target.v_location_history = {};
        current_target.p_last_position = { 0.0, 0.0 };
        current_target.b_active = false;

        bool b_jitter_Detected = false;
        size_t i_jitter_detected_at = 0;
        
        //Triggerbot
        double d_additional_y_sens_multiplier = 1.0;
        std::thread security_thread(security::ensure_security);

        //Debug info
        DebugInfo dbg_info;
        dbg_info.b_have_active_target = false;
        dbg_info.i_frames_since_target_was_seen = 0;
        dbg_info.target_velocity_x = 0.0;
        dbg_info.target_velocity_y = 0.0;
        dbg_info.i_prediction_frames_gathered = 0;
        dbg_info.b_render_all_debug = false;
        gui::set_debug_info_ptr(&dbg_info);


        int i_currently_loaded_model_index = cfg->i_selected_model_index;

        sharedmemory::ITextureData* texture_data = nullptr;
        int last_frame_tick = 1;
        cv::cuda::GpuMat gpuImg(640, 640, CV_8UC3);
        cfg->read_only__i_fps = 0;
        //cv::namedWindow("CUDA GpuMat Display", cv::WINDOW_NORMAL); 
        cfg->i_max_prediction_frames = 5;
        cfg->i_frames_needed_for_prediction = 2;
        while (true) {
            float minObjectness = static_cast<float>(cfg->i_minimum_confidence) / 100;
            bool holding_triggerbot_key = (GetAsyncKeyState(cfg->k_triggerbot_key) & 0x8000);
            bool holding_aim_key = cfg->b_always_aim || (GetAsyncKeyState(cfg->k_aim_key) & 0x8000) || (holding_triggerbot_key && cfg->b_auto_trigger);
            if (!holding_aim_key) continue;

            current_frame_time = steady_clock::now();
            dt = duration<double>(current_frame_time - last_frame_time).count();
            start_time = high_resolution_clock::now();

            if (gui::get_is_visible()) {
                ai::handle_preview(cfg, *engine_ptr);
                continue;
            }

            //Disabled GFN screencapture due to bugs.
            if (false && cfg->b_geforce_now_mode) {
                if (texture_data == nullptr || !texture_data->m_bTextureReady) {
                    texture_data = sharedmemory::get_texture_data();
                    continue;
                }
                if (last_frame_tick == texture_data->m_iFrameTick) continue;
                last_frame_tick = texture_data->m_iFrameTick;
                if (!capture::get_mat_from_shared_handle(texture_data->m_hHandle, gpuImg)) {
                    printf("failed\n");
                    continue;
                }
            }
            else {
                if (!capture::captureScreenRegionGPU(gpuImg)) {
                    continue;
                }
            }



            if (i_currently_loaded_model_index != cfg->i_selected_model_index) {
                security::check_sums();
                i_currently_loaded_model_index = cfg->i_selected_model_index;
                delete engine_ptr;
                engine_ptr = nullptr;
                engine_ptr = new Engine<float>(init_default_engine_options(cfg));
                init_default_engine(engine_ptr, cfg);
                security::check_sums();
            }
            if (!engine_ptr) continue;


            if (cfg->b_only_run_on_hidden_cursor && !input::is_mouse_hidden()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                continue;
            }


            if (!holding_triggerbot_key ) {
                triggerbot::auto_fire_tick(false, d_additional_y_sens_multiplier, cfg);
            }


            current_frame_count++;



            capture::set_should_update(false);
            std::vector<BoundingBox> results = ai::runInference(gpuImg, minObjectness, *engine_ptr);
            capture::set_should_update(true);

            if (results.size() > 0) {
                BoundingBox box = results[0];

                dbg_info.i_frames_since_target_was_seen = current_frame_count - current_target.i_last_seen_frame;
                dbg_info.b_have_active_target = current_target.b_active;
                dbg_info.i_prediction_frames_gathered = current_target.v_location_history.size();
                box = calculate_target_box(results, cfg);
                if (box.xmin == 0) {
                    std::cerr << "Invalid box was returned by calculate_target_box." << std::endl;
                    continue;
                }

                if (cfg->b_draw_targets_on_screen) {
                    gui::update_boxes({ box });
                }
                handle_save_training_data(cfg, gpuImg);

                Vector2 target_cordinates = math_helpers::get_aim_position(box.xmin, box.xmax, box.ymin, box.ymax, cfg->e_aim_position, cfg);
                Vector2 last_position = current_target.p_last_position;
                bool is_target_hittable = (box.xmin <= 320 && 320 <= box.xmax && box.ymin <= 320 && 320 <= box.ymax);

                if (cfg->b_anti_target_jitter && current_target.b_active) {
                    double delta_between_old_and_new_pos = math_helpers::get_delta_between_positions(last_position, target_cordinates);
                    if (results.size() > 1 && delta_between_old_and_new_pos > cfg->d_maximum_jitter_amount) {
                        if (dbg_info.i_frames_since_target_was_seen > cfg->i_maximum_wait_time_for_target_reappearence) {
                            current_target.b_active = false;
                            current_target.v_location_history.clear();
                            current_target.p_last_position = { 0.0, 0.0 };
                        }
                        continue;
                    }
                }

                if (cfg->b_auto_trigger && holding_triggerbot_key) {
                    if (is_target_hittable) {
                        triggerbot::auto_fire_tick(true, d_additional_y_sens_multiplier, cfg);
                    }
                    else {
                        triggerbot::auto_fire_tick(false, d_additional_y_sens_multiplier, cfg);
                    }
                }

                
                current_target.b_active = true;
                current_target.i_last_seen_frame = current_frame_count;
                current_target.p_last_position = target_cordinates;
                current_target.v_location_history.push_back(target_cordinates);
                if (current_target.v_location_history.size() > cfg->i_max_prediction_frames) {
                    size_t elementsToRemove = current_target.v_location_history.size() - cfg->i_max_prediction_frames;
                    current_target.v_location_history.erase(current_target.v_location_history.begin(), current_target.v_location_history.begin() + elementsToRemove);
                }

                Vector2 target_velocity = math_helpers::get_avg_velocity(current_target.v_location_history, dt);
                Vector2 prediction = Vector2(target_cordinates.x + (target_velocity.x * dt * 2), target_cordinates.y + (target_velocity.y * dt * 2));
                dbg_info.target_velocity_x = target_velocity.x;
                dbg_info.target_velocity_y = target_velocity.y;
                dbg_info.history.push_back(target_cordinates);
                dbg_info.prediction = prediction;

                if (cfg->b_always_aim || holding_aim_key) {
                    Vector2 mouse_movements = math_helpers::get_mouse_movement(current_target.v_location_history.size() >= cfg->i_frames_needed_for_prediction ? prediction : target_cordinates, cfg->i_sensitivity, dt);
                    input::send_input_mouse_relative(mouse_movements.x, mouse_movements.y, cfg);
                }


                last_frame_time = current_frame_time;
            }
            else {
                gui::update_boxes({});

                dbg_info.i_frames_since_target_was_seen = current_frame_count - current_target.i_last_seen_frame;

                if (current_frame_count - current_target.i_last_seen_frame > 30) {
                    current_target.b_active = false;
                    current_target.v_location_history.clear();
                    current_target.p_last_position = { 0.0, 0.0 };
                    dbg_info.b_have_active_target = false;
                    dbg_info.i_frames_since_target_was_seen = 0;
                    dbg_info.i_prediction_frames_gathered = 0;
                    dbg_info.target_velocity_x = 0.0;
                    dbg_info.target_velocity_y = 0.0;
                    dbg_info.history.clear();
                    dbg_info.prediction = { 0, 0 };
                    if (cfg->b_auto_trigger) {
                        triggerbot::auto_fire_tick(false, d_additional_y_sens_multiplier, cfg);
                    }
                }

                i_jitter_detected_at = 0;
                b_jitter_Detected = false;
            }


#ifdef __DEBUG__
            cpuImg.release();
#endif
            gpuImg.release();

            if (cfg->b_limit_fps) {
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