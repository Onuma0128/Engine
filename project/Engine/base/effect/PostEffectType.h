#pragma once

#include "Vector3.h"
#include "Matrix4x4.h"

/// <summary>
/// ポストエフェクトの種類
/// </summary>
enum class PostEffectType {
    None,
    RenderTexture,
    Grayscale,
    Vignette,
    Smoothing,
    OutLine,
    OutLineMask,
    Count
};

/// <summary>
/// グレースケールエフェクトのデータ
/// </summary>
struct GrayscaleData
{
    Vector3 color;
    float t;
};

/// <summary>
/// ビネットエフェクトのデータ
/// </summary>
struct VignetteData
{
    float scale;
    float gamma;
    float padding[2];
};

/// <summary>
/// アウトラインエフェクトのデータ
/// </summary>
struct OutlineData
{
    Matrix4x4 projection;
};