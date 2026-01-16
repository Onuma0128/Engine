#pragma once

#include "Sprite.h"

// 前方宣言
class BossEnemy;

/// <summary>
/// ボスのHPバーUI表示クラス
/// </summary>
class BossHpBarUI : public Sprite
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
	void Update(const Vector2& position);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// セッター
	/// </summary>
	/// <param name="boss"></param>
	void SetBossEnemy(BossEnemy* boss) { boss_ = boss; }

private:

	// ボスのポインタ
	BossEnemy* boss_ = nullptr;

	// Textureの元のサイズ
	float preTextureSize_ = 0.0f;

};

