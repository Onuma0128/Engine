#pragma once

#include <memory>
#include <vector>

#define NOMINMAX
#include "Sprite.h"
#include "Vector2.h"
#include "Vector4.h"

/// <summary>
/// numbers.pngの連番数字スプライトで数値を表示するUIクラス
/// </summary>
class NumbersUI
{
public:
    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="maxDigits">表示できる最大桁数</param>
    void Initialize(int maxDigits = 3);

    /// <summary>
    /// 終了処理
    /// </summary>
    void Finalize();

    /// <summary>
    /// 表示する数値で更新
    /// </summary>
    void Update(int number);

    /// <summary>
    /// 表示位置を設定
    /// </summary>
    void SetPosition(const Vector2& position) { position_ = position; }

    /// <summary>
    /// 1文字あたりの描画サイズを設定
    /// </summary>
    void SetDigitSize(const Vector2& digitSize) { digitSize_ = digitSize; }

    /// <summary>
    /// 文字間隔を設定
    /// </summary>
    void SetDigitSpacing(float digitSpacing) { digitSpacing_ = digitSpacing; }

    /// <summary>
    /// カラーを設定
    /// </summary>
    void SetColor(const Vector4& color) { color_ = color; }

    /// <summary>
    /// 表示/非表示を設定
    /// </summary>
    void SetVisible(bool isVisible);

private:
    void ApplyDigitSprite(Sprite* sprite, int digit, int index);

private:
    static constexpr float kSourceDigitWidth = 128.0f;
    static constexpr float kSourceDigitHeight = 128.0f;

    std::vector<std::unique_ptr<Sprite>> digitSprites_;

    Vector2 position_ = { 40.0f, 40.0f };
    Vector2 digitSize_ = { 64.0f, 64.0f };
    Vector4 color_ = { 1.0f, 1.0f, 1.0f, 1.0f };

    float digitSpacing_ = 4.0f;
    int maxDigits_ = 0;
};