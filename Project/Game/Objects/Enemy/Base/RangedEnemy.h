#pragma once

#include <memory>

#include "BaseEnemy.h"

#include "Objects/Enemy/Bullet/EnemyBullet.h"

/// <summary>
/// 弱遠距離敵の派生クラス
/// </summary>
class RangedEnemy : public BaseEnemy
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 死亡時呼び出し
	/// </summary>
	void Dead() override;

	/// <summary>
	/// リセット時呼び出し
	/// </summary>
	/// <param name="position"></スポーンする座標を入れる>
	void Reset(const Vector3& position) override;

	/// <summary>
	/// 弾のゲッター
	/// </summary>
	/// <returns></returns>
	std::vector<std::unique_ptr<EnemyBullet>>& GetBullets() override { return bullets_; }

private:

	std::vector<std::unique_ptr<EnemyBullet>> bullets_;

};

