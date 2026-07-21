#pragma once

#include <memory>

#define NOMINMAX
#include "Sprite.h"
#include "Vector2.h"
#include "Vector4.h"

/// <summary>
/// 2枚のSpriteで肩からひじ、ひじから手首までの腕を再現するUIクラス
/// </summary>
class DumbbellArmUI
{
public:
    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize();

    /// <summary>
    /// 終了処理
    /// </summary>
    void Finalize();

    /// <summary>
    /// 腕の曲げ角度で更新する（0度で伸びた状態）
    /// </summary>
    void Update(float angleDegrees);

    /// <summary>
    /// 上腕の向きを設定
    /// </summary>
    void SetUpperArmAngle(float upperArmAngleDegrees) { upperArmAngleDegrees_ = upperArmAngleDegrees; }

    /// <summary>
    /// 腕の長さを設定
    /// </summary>
    void SetArmLength(float upperArmLength, float forearmLength);

    /// <summary>
    /// 腕の色を設定
    /// </summary>
    void SetColor(const Vector4& color) { color_ = color; }

private:
    void SetupArmSprite(Sprite* sprite) const;
    void UpdateArmSprite(Sprite* sprite, const Vector2& startPosition, float length,float thickness, float rotateRadians) const;
    Vector2 CalculateEndPosition(const Vector2& startPosition, float length, float rotateRadians) const;
    float ToRadians(float degrees) const;

private:
    std::unique_ptr<Sprite> upperArmSprite_ = nullptr;
    std::unique_ptr<Sprite> forearmSprite_ = nullptr;

    Vector2 upperArmPosition_ = { 640.0f, 120.0f };
    Vector2 foreArmPosition_ = { 16.0f, -39.0f };
    Vector4 color_ = { 1.0f, 0.72f, 0.48f, 1.0f };
	Vector2 anchorPoint_ = { 0.1f, 0.35f };

    float upperArmLength_ = 228.0f;
    float forearmLength_ = 252.0f;
    float upperArmThickness_ = 120.0f;
    float foreArmThickness_ = 120.0f;

    float upperArmAngleDegrees_ = 90.0f;
};