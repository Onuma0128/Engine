#pragma once

#include <memory>

#include "BaseEnemy.h"

#include "objects/enemy/weapon/EnemyWeaponBase.h"

/// <summary>
/// シールド持ち近接敵の派生クラス
/// </summary>
class ShieldBearerEnemy : public BaseEnemy
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
	/// 描画をしていない時の更新
	/// </summary>
	void TransformUpdate() override;

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

	/// <summary>
	/// シールドウエポンのゲッター
	/// </summary>
	/// <returns></returns>
	EnemyWeaponBase* GetShieldWeapon() override { return shieldWeapon_.get(); }

private:

	std::unique_ptr<EnemyWeaponBase> weapon_ = nullptr;
	std::unique_ptr<EnemyWeaponBase> shieldWeapon_ = nullptr;

};

