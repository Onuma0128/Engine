// Vector2.cpp
#include "Vector2.h"

const Vector2 Vector2::UnitX = { 1.0f, 0.0f };
const Vector2 Vector2::UnitY = { 0.0f, 1.0f };
const Vector2 Vector2::Zero = { 0.0f, 0.0f };

Vector2::Vector2(float x, float y) : x(x), y(y) {}

Vector2 Vector2::Add(const Vector2& v1, const Vector2& v2) {
    return { v1.x + v2.x, v1.y + v2.y };
}

Vector2 Vector2::Subtract(const Vector2& v1, const Vector2& v2) {
    return { v1.x - v2.x, v1.y - v2.y };
}

Vector2 Vector2::Multiply(float scalar, const Vector2& v) {
    return { v.x * scalar, v.y * scalar };
}

float Vector2::Length() const {
    return std::sqrt(x * x + y * y);
}

float Vector2::Dot(const Vector2& v1, const Vector2& v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

Vector2 Vector2::Lerp(const Vector2& v1, const Vector2& v2, float t) {
    t = std::clamp(t, 0.0f, 1.0f);
    return {
        v1.x + (v2.x - v1.x) * t,
        v1.y + (v2.y - v1.y) * t
    };
}

Vector2 Vector2::EaseLerp(const Vector2& v1, const Vector2& v2, float t)
{
    return {
         v1.x + (v2.x - v1.x) * t,
         v1.y + (v2.y - v1.y) * t
    };
}

Vector2 Vector2::Rotate(const Vector2& v, float rotate)
{
    float c = std::cosf(rotate);
    float s = std::sinf(rotate);
    return Vector2{
        v.x * c - v.y * s,
        v.x * s + v.y * c
    };
}

Vector2 Vector2::Normalize() const {
    float len = Length();
    assert(len != 0 && "Zero division during normalization.");
    return { x / len, y / len };
}


Vector2 Vector2::MochiPuniScaleNormalized(float time, float amplitude, float damping)
{
    time = std::clamp(time, 0.0f, 1.0f);

    const float pi = 3.14159265358979323846f;
    const float twoPi = 6.2831853071795864769f;

    // 0と1で必ず0になる包絡
    float envelope = std::sin(pi * time);

    // 減衰（0ならそのまま）
    float decay = (damping > 0.0f) ? std::exp(-damping * time) : 1.0f;

    // X: 通常のsin波, Y: 位相を進める（ただし必ずenvelopeで0終端）
    float sx = 1.0f + amplitude * std::sin(twoPi * time) * envelope * decay;
    float sy = 1.0f + amplitude * std::sin(twoPi * time + pi * 0.5f) * envelope * decay;

    return { sx, sy };
}


Vector2 Vector2::operator+() const {
    return *this;
}

Vector2 Vector2::operator-() const {
    return { -x, -y };
}

Vector2 Vector2::operator+(const Vector2& v) const {
    return { x + v.x, y + v.y };
}

Vector2 Vector2::operator-(const Vector2& v) const {
    return { x - v.x, y - v.y };
}

Vector2 Vector2::operator*(float scalar) const {
    return { x * scalar, y * scalar };
}

Vector2& Vector2::operator+=(const Vector2& v) {
    x += v.x;
    y += v.y;
    return *this;
}

Vector2& Vector2::operator-=(const Vector2& v) {
    x -= v.x;
    y -= v.y;
    return *this;
}

Vector2& Vector2::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}

Vector2 operator*(float scalar, const Vector2& v) {
    return v * scalar;
}
