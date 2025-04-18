#include "PlayerBullet.h"

void PlayerBullet::Init(const WorldTransform& transform)
{
	Object3d::Initialize("Box.obj");

	// 回転と座標を取得
	transform_.rotation_ = transform.rotation_;
	transform_.translation_ = transform.translation_;

	// 速度(向き)を取得
	Matrix4x4 rotateMatrix = Quaternion::MakeRotateMatrix(transform.rotation_);
	velocity_ = Vector3::ExprUnitZ.Transform(rotateMatrix);
}

void PlayerBullet::GlobalInit()
{
}

void PlayerBullet::Update()
{



	Object3d::Update();
}

void PlayerBullet::Draw()
{
	Object3d::Draw();
}
