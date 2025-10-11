#pragma once

#include "Object3d.h"

class Player;

class PredictionObject :public Object3d
{
public:

	void Init();

	void Update(const Vector3& offset);

	void SetPlayer(Player* player) { player_ = player; }

private:

	Player* player_ = nullptr;
	
};
