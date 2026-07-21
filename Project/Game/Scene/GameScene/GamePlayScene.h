#pragma once

#include <memory>

#include "BaseScene.h"
#include "BaseUI.h"

#include "DumbbellCounter/DumbbellSensorController.h"
#include "Uis/NumbersUI.h"
#include "Uis/DumbbellArmUI.h"
#include "Uis/CheerForUI.h"

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

	// ダンベルセンサーの制御クラス
    std::unique_ptr<DumbbellSensorController> dumbbellSensorController_;
    
    // アームカールのカウント用UI
    std::unique_ptr<NumbersUI> numbersUI_;

	// 曲げ伸ばしをする腕
	std::unique_ptr<DumbbellArmUI> dumbbellArm_;

	// 腕の曲げ伸ばしのUI
	std::unique_ptr<CheerForUI> cheerForUI_;

	// 腕の曲げ伸ばしの設定用UI
    std::unique_ptr<BaseUI> armSettingUI_ = nullptr;

};