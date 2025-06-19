#pragma once

#include "gameScene/enemy/weapon/EnemyWeaponBase.h"

class EnemyShield : public EnemyWeaponBase
{
public:

	EnemyShield(Enemy* enemy);

	void Init(ColliderType type, const std::string& name) override;

	void Update() override;

	void OnCollisionEnter(Collider* other) override;
	void OnCollisionStay(Collider* other) override;
	void OnCollisionExit(Collider* other) override;

	void SetIsActive(bool flag) override;
};

