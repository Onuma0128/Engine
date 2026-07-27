#pragma once

#include "Sprite.h"

/// <summary>
/// プレイヤーの弾を表示するUI
/// </summary>
class PlayerBulletUI : public NumaEngine::Sprite
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="position"></param>
	void Init(const NumaEngine::Vector2& position);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="size"></param>
	/// <param name="position"></param>
	void Update(const NumaEngine::Vector2& size, const NumaEngine::Vector2& position);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:

};


