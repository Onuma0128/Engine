#pragma once
#include <cmath>
#include <cassert>
#include <vector>

#include "Matrix4x4.h"
#include "Quaternion.h"

/// <summary>
/// 3Dベクトルを表す構造体
/// </summary>
class Vector3 
{
public:
    float x, y, z;

    // コンストラクタ
    Vector3(float x = 0, float y = 0, float z = 0);

    // 定数定義
    static const Vector3 ExprUnitXYZ;
    static const Vector3 ExprUnitX;
    static const Vector3 ExprUnitY;
    static const Vector3 ExprUnitZ;
    static const Vector3 ExprZero;

    // 加算
    static Vector3 Add(const Vector3& v1, const Vector3& v2);

    // 減算
    static Vector3 Subtract(const Vector3& v1, const Vector3& v2);

    // スカラー倍
    static Vector3 Multiply(float scalar, const Vector3& v);

    // 長さ
    float Length()const;

    // 距離
    static float Distance(const Vector3& v1, const Vector3& v2);

    // 内積
    static float Dot(const Vector3& v1, const Vector3& v2);

    // 外積
    static Vector3 Cross(const Vector3& v1, const Vector3& v2);

    static Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);

    // 正規化
    Vector3 Normalize() const;

    // 行列による変換
    static Vector3 Transform(const Vector3& v, const Matrix4x4& matrix);
    Vector3 Transform(const Matrix4x4& matrix) const;

    static Vector3 TransformNormal(const Vector3& v, const Matrix4x4& matrix);
    Vector3 TransformNormal(const Matrix4x4& matrix)const;

    static float AxisComponent(const Vector3& v, int axis);

    static Quaternion FromEuler(const Vector3& eulerDeg);

    static Vector3 CatmullRomInterpolation(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t);

    static Vector3 CatmullRomPosition(const std::vector<Vector3>& points, float t);

    // 単項演算子オーバーロード
    Vector3 operator+() const;
    Vector3 operator-() const;

    // 二項演算子オーバーロード
    Vector3 operator+(const Vector3& v) const;
    Vector3 operator-(const Vector3& v) const;
    Vector3 operator*(float scalar) const;

    // 複合代入演算子オーバーロード
    Vector3& operator+=(const Vector3& v);
    Vector3& operator-=(const Vector3& v);
    Vector3& operator*=(float scalar);
	bool operator==(const Vector3& v);

    // フレンド関数：スカラー倍の演算子オーバーロード
    friend Vector3 operator*(float scalar, const Vector3& v);

    float& operator[](int i);             // 書き込み用
    const float& operator[](int i) const; // 読み取り用
};