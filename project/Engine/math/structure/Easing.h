#pragma once
#include <cmath>
#include <numbers>
#include <algorithm>

#include <string>
#include <array>

enum class EasingType : int32_t {
    Linear = 0,
    EaseInSine, EaseOutSine, EaseInOutSine,
    EaseInQuad, EaseOutQuad, EaseInOutQuad,
    EaseInCubic, EaseOutCubic, EaseInOutCubic,
    EaseInQuart, EaseOutQuart, EaseInOutQuart,
    EaseInQuint, EaseOutQuint, EaseInOutQuint,
    EaseInExpo, EaseOutExpo, EaseInOutExpo,
    EaseInCirc, EaseOutCirc, EaseInOutCirc,
    EaseInBack, EaseOutBack, EaseInOutBack,
};

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

    static constexpr const std::array<const char*, 25>& GetEaseTypeNames() { return easeTypes_; }

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
        case EasingType::Linear:         return t;

        case EasingType::EaseInSine:     return Easing::EaseInSine(t);
        case EasingType::EaseOutSine:    return Easing::EaseOutSine(t);
        case EasingType::EaseInOutSine:  return Easing::EaseInOutSine(t);

        case EasingType::EaseInQuad:     return Easing::EaseInQuad(t);
        case EasingType::EaseOutQuad:    return Easing::EaseOutQuad(t);
        case EasingType::EaseInOutQuad:  return Easing::EaseInOutQuad(t);

        case EasingType::EaseInCubic:    return Easing::EaseInCubic(t);
        case EasingType::EaseOutCubic:   return Easing::EaseOutCubic(t);
        case EasingType::EaseInOutCubic: return Easing::EaseInOutCubic(t);

        case EasingType::EaseInQuart:    return Easing::EaseInQuart(t);
        case EasingType::EaseOutQuart:   return Easing::EaseOutQuart(t);
        case EasingType::EaseInOutQuart: return Easing::EaseInOutQuart(t);

        case EasingType::EaseInQuint:    return Easing::EaseInQuint(t);
        case EasingType::EaseOutQuint:   return Easing::EaseOutQuint(t);
        case EasingType::EaseInOutQuint: return Easing::EaseInOutQuint(t);

        case EasingType::EaseInExpo:     return Easing::EaseInExpo(t);
        case EasingType::EaseOutExpo:    return Easing::EaseOutExpo(t);
        case EasingType::EaseInOutExpo:  return Easing::EaseInOutExpo(t);

        case EasingType::EaseInCirc:     return Easing::EaseInCirc(t);
        case EasingType::EaseOutCirc:    return Easing::EaseOutCirc(t);
        case EasingType::EaseInOutCirc:  return Easing::EaseInOutCirc(t);

        case EasingType::EaseInBack:     return Easing::EaseInBack(t);
        case EasingType::EaseOutBack:    return Easing::EaseOutBack(t);
        case EasingType::EaseInOutBack:  return Easing::EaseInOutBack(t);
        }
    }


private:

    static constexpr std::array<const char*, 25> easeTypes_ = {
        "Linear",
        "EaseInSine", "EaseOutSine", "EaseInOutSine",
        "EaseInQuad", "EaseOutQuad", "EaseInOutQuad",
        "EaseInCubic","EaseOutCubic","EaseInOutCubic",
        "EaseInQuart","EaseOutQuart","EaseInOutQuart",
        "EaseInQuint","EaseOutQuint","EaseInOutQuint",
        "EaseInExpo", "EaseOutExpo", "EaseInOutExpo",
        "EaseInCirc", "EaseOutCirc", "EaseInOutCirc",
        "EaseInBack", "EaseOutBack", "EaseInOutBack"
    };

    static constexpr int kEaseCount = static_cast<int>(sizeof(easeTypes_) / sizeof(easeTypes_[0]));

};
