#include "../AI.h"

double i_last_sensitivity = 0;
int i_last_head_margin = 0;
float f_last_minimum_head_margin = 0.0f;

void ai::handle_preview(AimConfig *cfg, Engine<float>& engine_ptr) {
    return;
    /*
    if (i_last_sensitivity == cfg->i_sensitivity && i_last_head_margin == cfg->i_head_margin && cfg->f_minimum_top_margin == f_last_minimum_head_margin) {
        return;
    }
    i_last_sensitivity = cfg->i_sensitivity;
    i_last_head_margin = cfg->i_head_margin;
    f_last_minimum_head_margin = cfg->f_minimum_top_margin;

    cv::cuda::GpuMat gpuImg;
    cv::Mat cpuImg = preview::get_cached_preview_image();
    gpuImg.upload(cpuImg);
    std::vector<BoundingBox> results = ai::runInference(gpuImg, static_cast<float>(cfg->i_minimum_confidence) / 100, engine_ptr);
    if (results.size() == 0) return;

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
    security::check_sums();
    */
}