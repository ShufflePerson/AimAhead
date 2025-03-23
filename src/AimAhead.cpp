#include <chrono>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/opencv.hpp>
#include <Wincon.h>
#include "./Helpers/logger.h"
#include "./Engine/Engine.h"
#include "./Capture/Capture.h"
#include "./Structs/BoundingBox.h"
#include "./AI/AI.h"
#include "./Utils/Utils.h"
#include "./Debug/Debug.h"
#include "./Collector/Collector.h"
#include "./GUI/GUI.h"
#include "./ModelManager/ModelManager.h"
#include "./ConfigManager/ConfigManager.h"
#include "Structs/Config.h"
#include "Structs/CurrentTarget.h"
#include "./Security/Security.h"
#include "./sharedmemory/sharedmemory.h"
#include "./Injector/injector.h"

int main(int argc, char* argv[]) {

    std::thread security_thread(security::ensure_security);

    /*
    std::thread connection_thread(connection::init_connection);
    connection_thread.detach();
    /*if (!connection::wait_for_security_ok()) {
        security::breach_detected();
        return 0;
    }*/

    AimConfig cfg;
    model_manager::update_models_list();
    config_manager::load_config_or_init_new(&cfg);
    gui::load_config(&cfg);
    sharedmemory::init_sender();
    if (!capture::InitializeCapture()) {
        throw std::runtime_error(XorStr("Failed to start screen capture."));
        return 1;
    }

    std::thread all_threads(utils::start_threads);



    security::add_checksum_watch((PVOID)&ai::main_loop);
    security::add_checksum_watch((PVOID)&ai::runInference);
    security::add_checksum_watch((PVOID)&utils::generate_uuidv4);
    security::add_checksum_watch((PVOID)&utils::getHwndFromTitle);
    security::add_checksum_watch((PVOID)&utils::get_current_unix_time_s);
    security::add_checksum_watch((PVOID)&utils::get_primary_display_size);
    security::add_checksum_watch((PVOID)&utils::init_config_with_defaults);
    security::add_checksum_watch((PVOID)&utils::start_threads);
    security::add_checksum_watch((PVOID)&utils::write_to_file);
    security::add_checksum_watch((PVOID)&model_manager::get_all_models);
    security::add_checksum_watch((PVOID)&model_manager::get_loaded_models);
    security::add_checksum_watch((PVOID)&model_manager::update_models_list);
    security::add_checksum_watch((PVOID)&gui::get_config);
    security::add_checksum_watch((PVOID)&gui::get_is_visible);
    security::add_checksum_watch((PVOID)&gui::init_gui);
    security::add_checksum_watch((PVOID)&gui::load_config);
    security::add_checksum_watch((PVOID)&gui::update_boxes);
    security::add_checksum_watch((PVOID)&gui::__render__aimbot_tab__);
    security::add_checksum_watch((PVOID)&gui::__render__misc_tab__);
    security::add_checksum_watch((PVOID)&gui::__render__preview_tab__);
    security::add_checksum_watch((PVOID)&gui::__render__settings_tab__);
    security::add_checksum_watch((PVOID)&gui::__render__visuals_tab__);
    security::add_checksum_watch((PVOID)&triggerbot::auto_fire_tick);
    security::add_checksum_watch((PVOID)&math_helpers::find_closests);
    security::add_checksum_watch((PVOID)&math_helpers::get_center);
    security::add_checksum_watch((PVOID)&math_helpers::get_delta_between_positions);
    security::add_checksum_watch((PVOID)&math_helpers::squared_distance);

    ai::main_loop(&cfg);

    system("pause");
    return 0;
}


extern "C" {
    __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;// Optimus: force switch to discrete GPU
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;//AMD
}

//todo:
//proper anti jitter
//multiple esp