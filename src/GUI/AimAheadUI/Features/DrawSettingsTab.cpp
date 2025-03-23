#include "../AimAheadUI.h"
#include "../../../ModelManager/ModelManager.h"

static float f_fps_cap = 0.1f;
static float f_min_confidence = 0.1f;
static bool v1 = false;
void aimahead_ui::draw_settings_tab(AimConfig *cfg) {
    if (f_fps_cap == 0.1f) {
        f_fps_cap = static_cast<float>(cfg->i_fps_cap);
        f_min_confidence = static_cast<float>(cfg->i_minimum_confidence);
    }
    ImGui::AH_Checkbox_Prop checkbox_prop = get_default_checkbox_prop();
    ImGui::AH_Slider_Prop slider_prop = get_default_slider_prop();
    ImGui::AH_ButtonInfo_Prop buttoninfo_prop = get_default_buttoninfo_prop();

    ImVec2 performance_box_pos = draw_container_box(XorStr("PERFORMANCE"));
    ImGui::SetCursorPosX(performance_box_pos.x);
    ImGui::SetCursorPosY(performance_box_pos.y);
    ImGui::AH_Checkbox(XorStr("INT-8 Mode"), XorStr("Inference with INT8 instead of FP16"), &cfg->b_int8_mode, &checkbox_prop);

    ImGui::AH_Checkbox(XorStr("Limit FPS"), XorStr("Set a cap on max fps"), &cfg->b_limit_fps, &checkbox_prop);
    if (cfg->b_limit_fps) {
        if (ImGui::AH_Slider(XorStr("Max FPS"), &f_fps_cap, 1.0f, 240.0f, "FPS", &slider_prop)) {
            cfg->i_fps_cap = static_cast<int>(f_fps_cap);
        }
    }
    if (ImGui::AH_Slider(XorStr("Minimum Confidence"), &f_min_confidence, 1.0f, 100.0f, "%", &slider_prop)) {
        cfg->i_minimum_confidence = static_cast<int>(f_min_confidence);
    }

    std::vector<std::string> loaded_models = model_manager::get_loaded_models();
    ImGui::SetNextItemWidth(checkbox_prop.f_available_x_space);
    ImGui::ListBox(
        XorStr("##models_list"),
        &cfg->i_selected_model_index,
        [](void* data, int idx, const char** out_text) {
            std::vector<std::string>* vec = (std::vector<std::string>*)data;
            if (idx < 0 || idx >= vec->size()) { return false; }
            *out_text = vec->at(idx).c_str();
            return true;
        },
        (void*)&loaded_models,
        loaded_models.size(),
        5
    );

    ImVec2 menu_box_pos = draw_container_box(XorStr("MENU"));
    ImGui::SetCursorPosX(menu_box_pos.x);
    ImGui::SetCursorPosY(menu_box_pos.y);
    ImGui::AH_Checkbox(XorStr("Use new menu"), XorStr("Toggle between legacy/new menu"), &cfg->b_use_new_gui, &checkbox_prop);


    ImVec2 gfn_box_pos = draw_container_box(XorStr("GEFORCE NOW"));
    ImGui::SetCursorPosX(gfn_box_pos.x);
    ImGui::SetCursorPosY(gfn_box_pos.y);
    ImGui::AH_Checkbox(XorStr("Geforce Now Mode"), XorStr("Send inputs to GFN"), &cfg->b_geforce_now_mode, &checkbox_prop);
#ifdef COMPILE_GFN_MODE
    if (ImGui::AH_ButtonInfo(XorStr("Inject"), XorStr("Press before launching GFN"), XorStr("INJECT"), &v1, &buttoninfo_prop)) {
        if (injector::get_gfn_pid() != -1) {
            MessageBoxA(NULL, "Please click Inject before you launch Geforce Now.", "GFN Running!", MB_ICONERROR);
            return;
        }
        if (!injector::is_injected()) {
            std::thread injection_thread(injector::inject);
            injection_thread.detach();
        }
        else {
            MessageBoxA(NULL, "Already Injected. Restart to re-inject.", "Alerady Injected!", MB_ICONERROR);
        }
    }
#endif  
}