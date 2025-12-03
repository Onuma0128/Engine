#pragma once
#include <memory>

#include "BaseScene.h"

#include "Camera.h"
#include "Animation.h"
#include "scene/sceneFade/SceneFade.h"
#include "uis/gameClearUI/GameClearUI.h"

/// <summary>
/// ゲームクリアシーン
/// </summary>
class GameClearScene : public BaseScene
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

	std::shared_ptr<Camera> camera_ = nullptr;

	std::unique_ptr<SceneFade> sceneFade_ = nullptr;
	bool isFade_ = false;

	std::unique_ptr<Animation> enemy_ = nullptr;

	std::unique_ptr<GameClearUI> gameClearUI_ = nullptr;

};

