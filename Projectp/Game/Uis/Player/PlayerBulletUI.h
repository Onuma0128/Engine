#pragma once

#include "Sprite.h"

/// <summary>
/// プレイヤーの弾を表示するUI
/// </summary>
class PlayerBulletUI :public Sprite
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="position"></param>
	void Init(const Vector2& position);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="size"></param>
	/// <param name="position"></param>
	void Update(const Vector2& size, const Vector2& position);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:

};

