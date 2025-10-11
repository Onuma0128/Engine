#pragma once

#include <memory>

#include "BaseEnemy.h"

#include "objects/enemy/weapon/EnemyWeaponBase.h"

class ShieldBearerEnemy : public BaseEnemy
{
public:

	void Initialize() override;

	void Update() override;

	void TransformUpdate() override;

	void Draw() override;

	void Dead() override;
	void Reset(const Vector3& position) override;

	EnemyWeaponBase* GetWeapon() override { return weapon_.get(); }
	EnemyWeaponBase* GetShieldWeapon() override { return shieldWeapon_.get(); }

private:

	std::unique_ptr<EnemyWeaponBase> weapon_ = nullptr;
	std::unique_ptr<EnemyWeaponBase> shieldWeapon_ = nullptr;

};

