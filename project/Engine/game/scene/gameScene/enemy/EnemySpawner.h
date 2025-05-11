#pragma once
#include <list>
#include <memory>
#include "Enemy.h"

#include "Object3d.h"

class EnemySpawner : public Object3d
{
public:

	void Init();

	void Update();

	void Draw();

	std::list<std::unique_ptr<Enemy>>& GetEnemyList() { return enemys_; }

private:

	void EnemySpawn();

private:

	std::list<std::unique_ptr<Enemy>> enemys_;

};

