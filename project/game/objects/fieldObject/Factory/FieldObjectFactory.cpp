#include "FieldObjectFactory.h"

void FieldObjectFactory::Init(SceneJsonLoader loader)
{
	for (auto it = loader.GetData().begin(); it != loader.GetData().end();) {
		if (it->second.groupName == "FieldObject") {
			std::unique_ptr<FieldObject> object = std::make_unique<FieldObject>();
			object->Init(it->second);
			fieldObjects_.push_back(std::move(object));
		}
		++it;
	}
}

void FieldObjectFactory::Update()
{
	for (auto& object : fieldObjects_) {
		object->Update();
	}
}