#pragma once
#include <string>

#include "Object3d.h"
#include "Collider.h"

class BaseEnemy;

class EnemyWeaponBase : public Object3d, public Collider
{
public:

	~EnemyWeaponBase() = default;
	EnemyWeaponBase(BaseEnemy* enemy);

	virtual void Finalize() = 0;

	virtual void Init(ColliderType type, const std::string& name) = 0;

	virtual void Update() = 0;

	virtual void SetIsActive(bool flag) = 0;
	virtual bool GetIsActive() = 0;

protected:

	BaseEnemy* enemy_ = nullptr;

};

