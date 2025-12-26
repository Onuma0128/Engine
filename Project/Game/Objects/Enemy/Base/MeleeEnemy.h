#pragma once

#include <memory>

#include "BaseEnemy.h"

#include "Objects/Enemy/Weapon/EnemyWeaponBase.h"

/// <summary>
/// 近接敵の派生クラス
/// </summary>
class MeleeEnemy : public BaseEnemy
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
	/// ウエポンのゲッター
	/// </summary>
	/// <returns></returns>
	EnemyWeaponBase* GetWeapon() override { return weapon_.get(); }

private:

	std::unique_ptr<EnemyWeaponBase> weapon_ = nullptr;

};

