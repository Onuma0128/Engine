#pragma once

#include <cmath>

class Vector4 {
public:

	float x, y, z, w;

	Vector4(float x = 0, float y = 0, float z = 0, float w = 0);

    // 定数
    static const Vector4 ExprUnitX;
    static const Vector4 ExprUnitY;
    static const Vector4 ExprUnitZ;
    static const Vector4 ExprUnitW;
    static const Vector4 ExprZero;

    static Vector4 Lerp(const Vector4& v1, const Vector4& v2, float t);

    // 単項演算子オーバーロード
    Vector4 operator-() const;

    // 二項演算子オーバーロード
    Vector4 operator+(const Vector4& v) const;
    Vector4 operator-(const Vector4& v) const;
    Vector4 operator*(float scalar) const;

    // 複合代入演算子オーバーロード
    Vector4& operator+=(const Vector4& v);
    Vector4& operator-=(const Vector4& v);
    Vector4& operator*=(float scalar);
};