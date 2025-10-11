#pragma once

#include "Object3d.h"

class CharacterShadow : public Object3d
{
public:

	void Init(const WorldTransform& transform);
	void Init();

	void Update();

	void SetDraw(bool flag) { Object3d::GetMaterial().enableDraw = flag; }
	void SetPosition(const Vector3& position) { transform_.translation_ = position; }
};

