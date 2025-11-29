#pragma once
#include <cmath>
#include <numbers>
#include <algorithm>

#define NOMINMAX

#include <string>
#include <array>

// Easing関数の種類
enum class EasingType : int32_t {
    kLinear = 0,
    kEaseInSine, kEaseOutSine, kEaseInOutSine,
    kEaseInQuad, kEaseOutQuad, kEaseInOutQuad,
    kEaseInCubic, kEaseOutCubic, kEaseInOutCubic,
    kEaseInQuart, kEaseOutQuart, kEaseInOutQuart,
    kEaseInQuint, kEaseOutQuint, kEaseInOutQuint,
    kEaseInExpo, kEaseOutExpo, kEaseInOutExpo,
    kEaseInCirc, kEaseOutCirc, kEaseInOutCirc,
    kEaseInBack, kEaseOutBack, kEaseInOutBack,
    kEaseOutBounce,
};

/// <summary>
/// Easing関数を提供するクラス
/// </summary>
class Easing {
public:
    static float EaseInSine(float t) {
        return 1.0f - std::cos((t * std::numbers::pi_v<float>) / 2.0f);
    }

    static float EaseOutSine(float t) {
        return std::sin((t * std::numbers::pi_v<float>) / 2.0f);
    }

    static float EaseInOutSine(float t) {
        return -(std::cos(std::numbers::pi_v<float> * t) - 1.0f) / 2.0f;
    }

    static float EaseInQuad(float t) {
        return t * t;
    }

    static float EaseOutQuad(float t) {
        return 1.0f - (1.0f - t) * (1.0f - t);
    }

    static float EaseInOutQuad(float t) {
        return t < 0.5f ? 2.0f * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 2.0f) / 2.0f;
    }

    static float EaseInCubic(float t) {
        return t * t * t;
    }

    static float EaseOutCubic(float t) {
        return 1.0f - std::pow(1.0f - t, 3.0f);
    }

    static float EaseInOutCubic(float t) {
        return t < 0.5f ? 4.0f * t * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 3.0f) / 2.0f;
    }

    static float EaseInQuart(float t) {
        return t * t * t * t;
    }

    static float EaseOutQuart(float t) {
        return 1.0f - std::pow(1.0f - t, 4.0f);
    }

    static float EaseInOutQuart(float t) {
        return t < 0.5f ? 8.0f * t * t * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 4.0f) / 2.0f;
    }

    static float EaseInQuint(float t) {
        return t * t * t * t * t;
    }

    static float EaseOutQuint(float t) {
        return 1.0f - std::pow(1.0f - t, 5.0f);
    }

    static float EaseInOutQuint(float t) {
        return t < 0.5f ? 16.0f * t * t * t * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 5.0f) / 2.0f;
    }

    static float EaseInExpo(float t) {
        return t == 0.0f ? 0.0f : std::pow(2.0f, 10.0f * t - 10.0f);
    }

    static float EaseOutExpo(float t) {
        return t == 1.0f ? 1.0f : 1.0f - std::pow(2.0f, -10.0f * t);
    }

    static float EaseInOutExpo(float t) {
        if (t == 0.0f) return 0.0f;
        if (t == 1.0f) return 1.0f;
        return t < 0.5f
            ? std::pow(2.0f, 20.0f * t - 10.0f) / 2.0f
            : (2.0f - std::pow(2.0f, -20.0f * t + 10.0f)) / 2.0f;
    }

    static float EaseInCirc(float t) {
        return 1.0f - std::sqrt(1.0f - t * t);
    }

    static float EaseOutCirc(float t) {
        return std::sqrt(1.0f - std::pow(t - 1.0f, 2.0f));
    }

    static float EaseInOutCirc(float t) {
        return t < 0.5f
            ? (1.0f - std::sqrt(1.0f - std::pow(2.0f * t, 2.0f))) / 2.0f
            : (std::sqrt(1.0f - std::pow(-2.0f * t + 2.0f, 2.0f)) + 1.0f) / 2.0f;
    }

    static float EaseInBack(float t) {
        float c1 = 1.70158f;
        float c3 = c1 + 1.0f;
        return c3 * t * t * t - c1 * t * t;
    }

    static float EaseOutBack(float t) {
        float c1 = 1.70158f;
        float c3 = c1 + 1.0f;
        return 1.0f + c3 * std::pow(t - 1.0f, 3.0f) + c1 * std::pow(t - 1.0f, 2.0f);
    }

    static float EaseInOutBack(float t) {
        float c1 = 1.70158f;
        float c2 = c1 * 1.525f;

        return t < 0.5f
            ? (std::pow(2.0f * t, 2.0f) * ((c2 + 1.0f) * 2.0f * t - c2)) / 2.0f
            : (std::pow(2.0f * t - 2.0f, 2.0f) * ((c2 + 1.0f) * (t * 2.0f - 2.0f) + c2) + 2.0f) / 2.0f;
    }

    static float EaseOutBounce(float t) {
        const float n1 = 4.0f;
        const float d1 = 2.75f;

        if (t < 1.0f / d1) {
            return n1 * t * t;
        } else if (t < 2.0f / d1) {
            t -= 1.5f / d1;
            return n1 * t * t + 0.75f;
        } else if (t < 2.5f / d1) {
            t -= 2.25f / d1;
            return n1 * t * t + 0.9375f;
        } else {
            t -= 2.625f / d1;
            return n1 * t * t + 0.984375f;
        }
    }

    static constexpr const std::array<const char*, 26>& GetEaseTypeNames() { return easeTypes_; }

    static EasingType FromInt(int v) {
        if (v < 0) v = 0;
        if (v >= kEaseCount) v = 0;
        return static_cast<EasingType>(v);
    }

    static int ToInt(EasingType type) { return static_cast<int>(type); }

    static float Apply(EasingType type, float t) {
        t = std::clamp(t, 0.0f, 1.0f);
        switch (type) {
        default:
        case EasingType::kLinear:         return t;

        case EasingType::kEaseInSine:     return Easing::EaseInSine(t);
        case EasingType::kEaseOutSine:    return Easing::EaseOutSine(t);
        case EasingType::kEaseInOutSine:  return Easing::EaseInOutSine(t);

        case EasingType::kEaseInQuad:     return Easing::EaseInQuad(t);
        case EasingType::kEaseOutQuad:    return Easing::EaseOutQuad(t);
        case EasingType::kEaseInOutQuad:  return Easing::EaseInOutQuad(t);

        case EasingType::kEaseInCubic:    return Easing::EaseInCubic(t);
        case EasingType::kEaseOutCubic:   return Easing::EaseOutCubic(t);
        case EasingType::kEaseInOutCubic: return Easing::EaseInOutCubic(t);

        case EasingType::kEaseInQuart:    return Easing::EaseInQuart(t);
        case EasingType::kEaseOutQuart:   return Easing::EaseOutQuart(t);
        case EasingType::kEaseInOutQuart: return Easing::EaseInOutQuart(t);

        case EasingType::kEaseInQuint:    return Easing::EaseInQuint(t);
        case EasingType::kEaseOutQuint:   return Easing::EaseOutQuint(t);
        case EasingType::kEaseInOutQuint: return Easing::EaseInOutQuint(t);

        case EasingType::kEaseInExpo:     return Easing::EaseInExpo(t);
        case EasingType::kEaseOutExpo:    return Easing::EaseOutExpo(t);
        case EasingType::kEaseInOutExpo:  return Easing::EaseInOutExpo(t);

        case EasingType::kEaseInCirc:     return Easing::EaseInCirc(t);
        case EasingType::kEaseOutCirc:    return Easing::EaseOutCirc(t);
        case EasingType::kEaseInOutCirc:  return Easing::EaseInOutCirc(t);

        case EasingType::kEaseInBack:     return Easing::EaseInBack(t);
        case EasingType::kEaseOutBack:    return Easing::EaseOutBack(t);
        case EasingType::kEaseInOutBack:  return Easing::EaseInOutBack(t);

        case EasingType::kEaseOutBounce: return Easing::EaseOutBounce(t);
        }
    }


private:

    static constexpr std::array<const char*, 26> easeTypes_ = {
        "Linear",
        "EaseInSine", "EaseOutSine", "EaseInOutSine",
        "EaseInQuad", "EaseOutQuad", "EaseInOutQuad",
        "EaseInCubic","EaseOutCubic","EaseInOutCubic",
        "EaseInQuart","EaseOutQuart","EaseInOutQuart",
        "EaseInQuint","EaseOutQuint","EaseInOutQuint",
        "EaseInExpo", "EaseOutExpo", "EaseInOutExpo",
        "EaseInCirc", "EaseOutCirc", "EaseInOutCirc",
        "EaseInBack", "EaseOutBack", "EaseInOutBack",
        "EaseOutBounce"
    };

    static constexpr int kEaseCount = static_cast<int>(sizeof(easeTypes_) / sizeof(easeTypes_[0]));

};
