#pragma once

#include "Object3d.h"

class PlayerBullet : public Object3d
{
public:

	void Init(const WorldTransform& transform);

	void GlobalInit();

	void Update();

	void Draw();

private:

	Vector3 velocity_;

};

