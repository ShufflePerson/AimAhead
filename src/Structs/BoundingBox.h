#pragma once
struct BoundingBox {
    float xmin;
    float ymin;
    float xmax;
    float ymax;
    float confidence;
    bool is_target = false;
};