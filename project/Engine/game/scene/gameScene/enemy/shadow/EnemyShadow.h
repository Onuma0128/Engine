#pragma once

#include "Object3d.h"

class EnemyShadow : public Object3d
{
public:

	void Init(const WorldTransform& transform);

	void Update();

	void SetDraw(bool flag) { Object3d::GetMaterial().enableDraw = flag; }

};

