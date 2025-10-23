// Vector2.h
#pragma once
#include <cmath>
#include <cassert>
#include <algorithm>

/// <summary>
/// 2Dベクトルを表す構造体
/// </summary>
class Vector2 {
public:
    float x, y;

	// コンストラクタ
    Vector2(float x = 0.0f, float y = 0.0f);

	// 定数定義
    static const Vector2 UnitX;
    static const Vector2 UnitY;
    static const Vector2 Zero;

	// 加算
    static Vector2 Add(const Vector2& v1, const Vector2& v2);

	// 減算
    static Vector2 Subtract(const Vector2& v1, const Vector2& v2);

	// スカラー倍
    static Vector2 Multiply(float scalar, const Vector2& v);

	// 内積
    static float Dot(const Vector2& v1, const Vector2& v2);

	// 線形補間
    static Vector2 Lerp(const Vector2& v1, const Vector2& v2, float t);

	// イージング付き線形補間
    static Vector2 EaseLerp(const Vector2& v1, const Vector2& v2, float t);

	// 回転
    static Vector2 Rotate(const Vector2& v, float rotate);

	// ベクトルの長さを求める
    float Length() const;

	// 正規化
    Vector2 Normalize() const;

	// モチぷにスケール(正規化済み)
    static Vector2 MochiPuniScaleNormalized(float time, float amplitude = 0.2f, float damping = 0.15f);

	// 演算子オーバーロード
    Vector2 operator+() const;
    Vector2 operator-() const;
    Vector2 operator+(const Vector2& v) const;
    Vector2 operator-(const Vector2& v) const;
    Vector2 operator*(float scalar) const;

    Vector2& operator+=(const Vector2& v);
    Vector2& operator-=(const Vector2& v);
    Vector2& operator*=(float scalar);

    friend Vector2 operator*(float scalar, const Vector2& v);
};
