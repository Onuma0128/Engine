#pragma once
#include <string>

#include "Object3d.h"
#include "Collider.h"

class Enemy;

class EnemyWeaponBase : public Object3d, public Collider
{
public:

	~EnemyWeaponBase() = default;
	EnemyWeaponBase(Enemy* enemy);

	virtual void Finalize() = 0;

	virtual void Init(ColliderType type, const std::string& name) = 0;

	virtual void Update() = 0;

	virtual void SetIsActive(bool flag) = 0;

protected:

	Enemy* enemy_ = nullptr;

};

