#include "../AI.h"

BoundingBox ai::calculate_target_box(std::vector<BoundingBox> results, AimConfig* cfg) {
    BoundingBox box;
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

    return box;
}