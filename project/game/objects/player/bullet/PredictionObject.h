#pragma once

#include "Object3d.h"
#include "Collider.h"

class Player;

class PredictionObject :public Object3d,Collider
{
public:

	void Init();

	void Update(const Vector3& offset);

	void SetPlayer(Player* player) { player_ = player; }

private:

	Player* player_ = nullptr;
	
};
