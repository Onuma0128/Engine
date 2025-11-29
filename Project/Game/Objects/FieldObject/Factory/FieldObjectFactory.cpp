#include "FieldObjectFactory.h"

#include "SearchAlgorithm/Collision/MapCollision.h"
#include "GameCamera/GameCamera.h"

void FieldObjectFactory::Init(SceneJsonLoader loader)
{
	items_ = std::make_unique<FieldObjectAdjustItem>();
	items_->LoadItems();

	for (auto it = loader.GetData().begin(); it != loader.GetData().end();) {
		if (it->second.groupName == "FieldObject") {
			std::unique_ptr<FieldObject> object = std::make_unique<FieldObject>();
			object->Init(it->second);
			object->SetItems(items_.get());
			object->SetMapCollision(mapCollision_);
			object->SetGameCamera(gameCamera_);
			fieldObjects_.push_back(std::move(object));
		}
		++it;
	}
}

void FieldObjectFactory::Update()
{
	items_->Editor();

	for (auto& object : fieldObjects_) {
		object->Update();
	}
}