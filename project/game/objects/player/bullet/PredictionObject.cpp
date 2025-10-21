#include "PredictionObject.h"

#include "objects/player/Player.h"

void PredictionObject::Init()
{
	Object3d::Initialize("Box.obj");
	Object3d::SetSceneRenderer();
	Object3d::GetMaterial().outlineMask = true;
	Object3d::GetMaterial().outlineSceneColor = true;
	transform_.scale_ = { 0.1f,0.1f,0.1f };

	Collider::AddCollider();
	Collider::myType_ = ColliderType::OBB;
	Collider::colliderName_ = "PredictionObject";
	Collider::size_ = { 0.2f,0.2f,3.0f };
	Collider::isActive_ = true;
	Collider::targetColliderName_ = { "Enemy" };
	Collider::DrawCollider();
}

void PredictionObject::Update(const Vector3& offset)
{
	Matrix4x4 rotateMat = Quaternion::MakeRotateMatrix(player_->GetShot()->GetRightStickQua());
	Vector3 position = offset.Transform(rotateMat);
	transform_.translation_ = player_->GetTransform().translation_ + position;

	Collider::rotate_ = player_->GetShot()->GetRightStickQua();
	Collider::centerPosition_ = transform_.translation_;
	Collider::Update();
	Object3d::Update();
}
