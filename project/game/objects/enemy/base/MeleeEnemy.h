#pragma once

#include <memory>

#include "BaseEnemy.h"

#include "objects/enemy/weapon/EnemyWeaponBase.h"

class MeleeEnemy : public BaseEnemy
{
public:

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Dead() override;
	void Reset(const Vector3& position) override;

	EnemyWeaponBase* GetWeapon() override { return weapon_.get(); }

private:

	std::unique_ptr<EnemyWeaponBase> weapon_ = nullptr;

};

