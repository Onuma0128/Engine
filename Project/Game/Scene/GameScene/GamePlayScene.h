#pragma once

#include "BaseScene.h"
#include "DumbbellCounter/DumbbellSensorController.h"

/// <summary>
/// ゲームプレイシーン
/// </summary>
class GamePlayScene : public BaseScene
{
public:

    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize() override;

    /// <summary>
    /// 終了
    /// </summary>
    void Finalize() override;

    /// <summary>
    /// 更新
    /// </summary>
    void Update() override;

    /// <summary>
    /// 描画
    /// </summary>
    void Draw() override;

private:

    /// <summary>
    /// センサー判定で使用している伸ばした姿勢との角度を取得する
    /// </summary>
    float GetSensorExtendedAngle() const;

    /// <summary>
    /// センサー判定で使用している曲げた姿勢との角度を取得する
    /// </summary>
    float GetSensorBentAngle() const;

private:

    DumbbellSensorController dumbbellSensorController_;
    
};