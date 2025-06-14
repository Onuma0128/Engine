#include "Revolver.h"

#include "gameScene/player/Player.h"

void Revolver::Init()
{
	Object3d::Initialize("Revolver.obj");
	Object3d::SetSceneRenderer();
}

void Revolver::Update()
{
	Matrix4x4 rotateMat = Quaternion::MakeRotateMatrix(player_->GetRightStickQua());
	Vector3 offset = player_->GetItem()->GetRevolverData().offset;
	Vector3 position = offset.Transform(rotateMat);
	transform_.rotation_ = player_->GetRightStickQua();
	transform_.translation_ = player_->GetTransform().translation_ + position;

	Object3d::Update();
}
