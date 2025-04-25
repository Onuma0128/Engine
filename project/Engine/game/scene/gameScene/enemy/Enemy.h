#pragma once
#include <memory>

#include "Object3d.h"

#include "gameScene/enemy/effect/EnemyEffect.h"

class Enemy:public Object3d
{
public:

	void Init();

	void Update();

	void Draw();

	EnemyEffect* GetEffect() { return effect_.get(); }

private:

	bool isCollision = false;

	std::unique_ptr<EnemyEffect> effect_ = nullptr;

};

