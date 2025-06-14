#pragma once

#include "Object3d.h"

class Player;

class Revolver : public Object3d
{
public:

	void Init();

	void Update();

	void SetPlayer(Player* player) { player_ = player; }

private:

	Player* player_ = nullptr;

};

