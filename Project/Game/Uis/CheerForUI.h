#pragma once

#include <memory>

#include "Sprite.h"

#include "DumbbellCounter/DumbbellPoseCounter.h"

class CheerForUI
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
    /// 表示する数値で更新
    /// </summary>
    void Update(DumbbellPoseCounter::State state);

private:

    static constexpr float kSourceDigitWidth = 256.0f;
    static constexpr float kSourceDigitHeight = 64.0f;
	std::unique_ptr<Sprite> cheerForSprite_ = nullptr;


};

