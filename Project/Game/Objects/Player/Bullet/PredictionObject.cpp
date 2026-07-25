#include "PredictionObject.h"

#include "objects/player/Player.h"

void PredictionObject::Init()
{
	Object3d::Initialize("Box.obj");
	Object3d::SetSceneRenderer();
	Object3d::GetMaterial().enableDraw = false;
	Object3d::GetMaterial().outlineMask = true;
	Object3d::GetMaterial().shadowMap = false;
	Object3d::GetMaterial().outlineSceneColor = true;
	transform_.scale_ = { 0.1f,0.1f,0.1f };
}

void PredictionObject::Update(const NumaEngine::Vector3& offset)
{
	Matrix4x4 rotateMat = NumaEngine::Quaternion::MakeRotateMatrix(player_->GetShot()->GetRightStickQua());
	NumaEngine::Vector3 position = offset.Transform(rotateMat);
	transform_.translation_ = player_->GetTransform().translation_ + position;

	Object3d::Update();
}

