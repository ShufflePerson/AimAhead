#include "../AI.h"

BoundingBox ai::calculate_target_box(std::vector<BoundingBox>& results, AimConfig* cfg) {
    BoundingBox target_box;
    target_box.xmin = 0;

    if (cfg == nullptr) {
        std::cerr << XorStr("Error: AimConfig pointer is null in calculate_target_box. Returning default BoundingBox.") << std::endl;
        return target_box;
    }

    if (results.empty()) {
        std::cerr << XorStr("Info: No results provided. Returning default BoundingBox.") << std::endl;
        return target_box;
    }

    if (cfg->e_target_mode == OBJECTIVE) {
        float highestConf = 0.0f;
        for (int i = 0; i < results.size(); i++) {
            if (results[i].confidence >= highestConf) {
                target_box = results[i];
                highestConf = target_box.confidence;
            }
        }
    }
    else if (cfg->e_target_mode == CLOSEST) {
        target_box = math_helpers::find_closests(results);
    }
    else {
        std::cerr << XorStr("Warning: Unknown target mode in calculate_target_box. Returning default BoundingBox.") << std::endl;
    }

    return target_box;
}