#include "PredictionObjects.h"

void PredictionObjects::Init()
{
	for (auto& obj : predictionObjects_) {
		obj = std::make_unique<Object3d>();
		obj->Initialize("Box.obj");
		obj->SetSceneRenderer();
		obj->GetMaterial().outlineMask = true;
		obj->GetMaterial().outlineSceneColor = true;
		obj->SetTransformScale({ 0.1f,0.1f,0.1f });
	}
}

void PredictionObjects::Update(const Vector3& start, const Vector3& goal)
{
	for (size_t i = 0; i < predictionObjects_.size(); ++i) {
		
	}
}
