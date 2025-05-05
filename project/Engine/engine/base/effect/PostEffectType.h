#pragma once

#include "Vector3.h"

enum class PostEffectType {
    None,
    Grayscale,
    Vignette,
    Blur,
    Invert,
    Noise,
    Bloom,
    Count
};

struct GrayscaleData
{
    Vector3 color;
    float t;
};

struct VignetteData
{
    float scale;
    float gamma;
    float padding[2];
};