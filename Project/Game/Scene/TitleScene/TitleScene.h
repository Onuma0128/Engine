#pragma once

#include <memory>

#include "BaseScene.h"
#include "BaseUI.h"

class TitleScene : public BaseScene
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

    void UpdateTitleHeartbeat();

private:

    // フェード
    std::unique_ptr<BaseUI> sceneFade_ = nullptr;
	std::unique_ptr<BaseUI> backGround_ = nullptr;
    bool isFade_ = false;

	// ドアが開くアニメーション
    std::unique_ptr<BaseUI> doorOpenL_ = nullptr;
    std::unique_ptr<BaseUI> doorOpenR_ = nullptr;
	bool isDoorOpen_ = false;


	// タイトルスプライト
    std::unique_ptr<Sprite> titleSprite_ = nullptr;
    // 元のタイトルサイズ
    Vector2 titleBaseSize_ = {};
    // 鼓動用タイマー
    float heartbeatTimer_ = 0.0f;
    // 1回の鼓動周期
    float heartbeatDuration_ = 0.85f;
    // 鼓動の強さ
    float heartbeatScale_ = 0.1f;

};

