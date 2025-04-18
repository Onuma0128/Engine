#pragma once

#include "Object3d.h"

#include "gameScene/player/effect/PlayerEffect.h"

class Player : public Object3d
{
public:

	void Init();

	void GlobalInit();

	void Update();

	void Draw();

private:

	std::unique_ptr<PlayerEffect> effect_ = nullptr;

};