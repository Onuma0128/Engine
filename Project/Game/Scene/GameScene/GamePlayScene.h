#pragma once
#include <memory>
#include <future>
#include <string>

#include "BaseScene.h"

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

	std::future<std::string> postFuture_;
	std::string postResult_;
	std::string facultiesJson_;
	bool isRequesting_ = false;
	bool isLoaded_ = false;

};

