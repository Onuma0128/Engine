#include "PlayerBullet.h"

#include "DeltaTimer.h"

void PlayerBullet::Init()
{
	Object3d::Initialize("Box.obj");

	isActive_ = false;
	activeFrame_ = 0.0f;
}

void PlayerBullet::GlobalInit()
{
}

void PlayerBullet::Update()
{
	// フレームが60立ったらIsActiveをfalseにする
	if (isActive_) {
		activeFrame_ += DeltaTimer::GetDeltaTime() * 2.0f;
		if (activeFrame_ >= 1.0f) {
			activeFrame_ = 1.0f;
			isActive_ = false;
		}
	} 

	if (wasActive_ && !isActive_ && onDeactivatedCallback_) {
		onDeactivatedCallback_();
	}

	wasActive_ = isActive_;

	if (!isActive_) {
		Object3d::Update();
		return;
	}

	const float bulletSpeed = 20.0f;
	transform_.translation_ += velocity_ * DeltaTimer::GetDeltaTime() * bulletSpeed;

	Object3d::Update();
}

void PlayerBullet::Draw()
{
	if (!isActive_) {
		return;
	}
	Object3d::Draw();
}

void PlayerBullet::Reload()
{
	isReload_ = true;
}

void PlayerBullet::Attack(const WorldTransform& transform)
{
	// 回転と座標を取得
	transform_.scale_ = { 0.1f,0.1f ,0.3f };
	transform_.rotation_ = transform.rotation_;
	transform_.translation_ = transform.translation_;

	// 速度(向き)を取得
	Matrix4x4 rotateMatrix = Quaternion::MakeRotateMatrix(transform.rotation_);
	velocity_ = Vector3::ExprUnitZ.Transform(rotateMatrix);

	isActive_ = true;
	activeFrame_ = 0.0f;
	isReload_ = false;
}

void PlayerBullet::IsCollision()
{
	isObjectCollision_ = true;

	wasActive_ = false;
	isActive_ = false;
	activeFrame_ = 1.0f;
}

void PlayerBullet::SetOnDeactivateCallback(const std::function<void()>& callback)
{
	onDeactivatedCallback_ = callback;
}
