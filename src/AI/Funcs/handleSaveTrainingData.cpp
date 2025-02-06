#include "../AI.h"

void ai::handle_save_training_data(AimConfig *cfg, cv::cuda::GpuMat &gpuImg) {
    if (cfg->b_save_training_data && collector::can_save()) {
        cv::Mat cpuImg;
        gpuImg.download(cpuImg);
        cv::cvtColor(cpuImg, cpuImg, cv::COLOR_BGRA2RGB);
        collector::save_image(cpuImg);
    }
}