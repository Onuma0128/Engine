#pragma once

#include <memory>
#include <list>

#include "SceneJsonLoader.h"
#include "gameScene/fieldObject/FieldObject.h"

class FieldObjectFactory
{
public:

	void Init(SceneJsonLoader loader);

	void Update();

private:

	std::list<std::unique_ptr<FieldObject>> fieldObjects_;

};

