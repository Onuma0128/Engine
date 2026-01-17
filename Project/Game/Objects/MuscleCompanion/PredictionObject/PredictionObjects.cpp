#include "PredictionObjects.h"

#include "Objects/MuscleCompanion/AdjustItem/CompanionAdjustItem.h"

void PredictionObjects::Init()
{
	for (auto& obj : predictionObjects_) {
		obj = std::make_unique<Object3d>();
		obj->Initialize("Box.obj");
		obj->SetSceneRenderer();
		obj->GetMaterial().outlineMask = true;
		obj->GetMaterial().shadowMap = true;
		obj->GetMaterial().outlineSceneColor = true;
		obj->SetTransformScale({ 0.1f,0.1f,0.1f });
	}
}

void PredictionObjects::Update(const Vector3& start, const Vector3& goal)
{
	// データを取得する
	const auto& data = items_->GetEffectData();
	float scale = 1.0f / static_cast<float>(predictionObjects_.size() + 1);
	Vector3 velocity = (goal - start) * scale;

	for (size_t i = 0; i < predictionObjects_.size(); ++i) {
		Vector3 position = velocity * static_cast<float>(i + 1) + start;
		Vector3 prePosition = predictionObjects_[i]->GetTransform().translation_;
		position.y = 1.0f;
		prePosition.y = 1.0f;
		predictionObjects_[i]->SetTransformRotation(Quaternion::DirectionToQuaternion(Quaternion::IdentityQuaternion(), velocity, 1.0f));
		predictionObjects_[i]->SetTransformTranslation(Vector3::Lerp(prePosition, position, data.lerpSpeed));
	}
}

void PredictionObjects::SetDraw(bool isDraw)
{
	for (auto& obj : predictionObjects_) {
		obj->GetMaterial().enableDraw = isDraw;
		obj->Update();
	}
}