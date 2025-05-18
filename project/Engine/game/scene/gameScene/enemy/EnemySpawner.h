#pragma once
#include <list>
#include <memory>
#include "Enemy.h"

#include "Object3d.h"
#include "Collider.h"

class EnemySpawner : public Object3d,Collider
{
public:

	void Init();

	void Update();

	std::list<std::unique_ptr<Enemy>>& GetEnemyList() { return enemys_; }

private:

	void EnemySpawn();

private:

	std::list<std::unique_ptr<Enemy>> enemys_;

};

