#pragma once

#include "Object3d.h"
#include "Collider.h"
#include "SceneJsonLoader.h"

class FieldObject : public Object3d,Collider
{
public:

	void Init(SceneObject object);

	void Update();

};

