#pragma once

#include "objects/enemy/weapon/EnemyWeaponBase.h"

class EnemyShield : public EnemyWeaponBase
{
public:

	EnemyShield(BaseEnemy* enemy);

	void Finalize() override;

	void Init(ColliderType type, const std::string& name) override;

	void Update() override;

	void OnCollisionEnter(Collider* other) override;
	void OnCollisionStay(Collider* other) override;
	void OnCollisionExit(Collider* other) override;

	void SetIsActive(bool flag) override;
	bool GetIsActive()override;

private:

	Quaternion rotateY_;
	Matrix4x4 rotateMatrix_;

};

