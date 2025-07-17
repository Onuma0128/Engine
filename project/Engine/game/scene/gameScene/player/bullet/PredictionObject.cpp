#include "PredictionObject.h"

#include "gameScene/player/Player.h"

void PredictionObject::Init()
{
	Object3d::Initialize("Box.obj");
	Object3d::SetSceneRenderer();
	transform_.scale_ = { 0.1f,0.1f,0.1f };
}

void PredictionObject::Update(const Vector3& offset)
{
	Matrix4x4 rotateMat = Quaternion::MakeRotateMatrix(player_->GetShot()->GetRightStickQua());
	Vector3 position = offset.Transform(rotateMat);
	transform_.translation_ = player_->GetTransform().translation_ + position;

	Object3d::Update();
}
