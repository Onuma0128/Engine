#pragma once

#include <string>

namespace NumaEngine { class Vector3; class Quaternion; }

/// <summary>
/// 4x4行列を表す構造体
/// </summary>
class Matrix4x4 {
public:
    // 行列データ (4x4の2次元配列)
    float m[4][4];

    // コンストラクタ
    Matrix4x4();

    // MatrixのImGui
    void ImGuiMatrix();

    Matrix4x4 Transpose();

    // 単位行列の生成
    static Matrix4x4 Identity();

    static Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

    // 拡縮行列の生成
    static Matrix4x4 Scale(const NumaEngine::Vector3& scale);

    // 回転行列の生成 (各軸)
    static Matrix4x4 RotateX(float radian);
    static Matrix4x4 RotateY(float radian);
    static Matrix4x4 RotateZ(float radian);

    // 3軸の回転を合成した回転行列の生成
    static Matrix4x4 Rotate(const NumaEngine::Vector3& rotate);

    // 移動行列の生成
    static Matrix4x4 Translate(const NumaEngine::Vector3& translate);

    // 正規化回転行列
    static Matrix4x4 NormalizeRotation(const Matrix4x4& matrix);

    // 行列の積
    Matrix4x4 operator*(const Matrix4x4& other) const;

    // LookAt行列の生成
    static Matrix4x4 LookAt(const NumaEngine::Vector3& eye, const NumaEngine::Vector3& target, const NumaEngine::Vector3& up);

    static NumaEngine::Vector3 ExtractEulerAngles(const Matrix4x4& m);

    // アフィン変換行列の生成
    static Matrix4x4 Affine(const NumaEngine::Vector3& scale, const NumaEngine::Vector3& rotate, const NumaEngine::Vector3& translate);
    static Matrix4x4 Affine(const NumaEngine::Vector3& scale, const NumaEngine::Quaternion& rotate, const NumaEngine::Vector3& translate);

    // 逆行列の生成
    static Matrix4x4 Inverse(const Matrix4x4& matrix);

    // 投資投影行列の生成
    static Matrix4x4 PerspectiveFov(float fovY, float aspectRatio, float nearClip, float farClip);

    // 正射影行列の生成
    static Matrix4x4 Orthographic(float left, float right, float bottom, float top,float nearClip, float farClip);
    // 任意軸回転行列の生成
    static Matrix4x4 MakeRotateAxisAngle(const NumaEngine::Vector3& axis, float angle);

    // ある方向からある方向への回転行列
    static Matrix4x4 DirectionToDirection(const NumaEngine::Vector3& from, const NumaEngine::Vector3& to);

};



