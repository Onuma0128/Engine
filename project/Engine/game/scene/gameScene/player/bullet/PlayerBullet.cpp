#include "PlayerBullet.h"

#include "DeltaTimer.h"

void PlayerBullet::Init(const uint32_t count)
{
	Object3d::Initialize("Box.obj");
	transform_.scale_ = { 0.1f,0.1f ,0.3f };
	Object3d::GetRenderOptions().enabled = false;

	isActive_ = false;
	activeFrame_ = 0.0f;

	effect_ = std::make_unique<PlayerBulletEffect>();
	effect_->Init(count);

	Collider::AddCollider();
	Collider::myType_ = ColliderType::OBB;
	Collider::colliderName_ = "PlayerBullet";
	Collider::size_ = transform_.scale_;
	Collider::isActive_ = false;
	Collider::DrawCollider();
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
			Collider::isActive_ = false;
			Object3d::GetRenderOptions().enabled = false;
		}
	} 

	// コールバック関数
	if (wasActive_ && !isActive_ && onDeactivatedCallback_) {
		onDeactivatedCallback_();
		effect_->OnceBulletDeleteEffect(transform_);
		Object3d::GetRenderOptions().enabled = false;
	}
	wasActive_ = isActive_;

	// Activeがfalseならこの先を更新しない
	if (!isActive_) {
		Collider::Update();
		Object3d::Update();
		return;
	}

	// 移動処理
	const float bulletSpeed = 20.0f;
	transform_.translation_ += velocity_ * DeltaTimer::GetDeltaTime() * bulletSpeed;

	effect_->OnceBulletTrailEffect(transform_);
	Collider::rotate_ = transform_.rotation_;
	Collider::centerPosition_ = transform_.translation_;
	Collider::Update();
	Object3d::Update();
}

void PlayerBullet::OnCollisionEnter(Collider* other)
{
	// 敵と当たったらなエフェクトを出す
	if (other->GetColliderName() == "Enemy") {
		IsCollision();
		effect_->OnceBulletDeleteEffect(transform_);
		effect_->OnceBulletHitEffect(transform_);
	}
}

void PlayerBullet::Reload()
{
	isReload_ = true;
}

void PlayerBullet::Attack(const WorldTransform& transform)
{
	// 回転と座標を取得
	transform_.rotation_ = transform.rotation_;
	transform_.translation_ = transform.translation_;

	// 速度(向き)を取得
	Matrix4x4 rotateMatrix = Quaternion::MakeRotateMatrix(transform.rotation_);
	velocity_ = Vector3::ExprUnitZ.Transform(rotateMatrix);

	activeFrame_ = 0.0f;
	isReload_ = false;
	isActive_ = true;
	Collider::isActive_ = true;
	Object3d::GetRenderOptions().enabled = true;

	effect_->OnceBulletEffect(transform);
}

void PlayerBullet::IsCollision()
{
	activeFrame_ = 1.0f;
	wasActive_ = false;
	isActive_ = false;
	Collider::isActive_ = false;
	Object3d::GetRenderOptions().enabled = false;
}

void PlayerBullet::SetOnDeactivateCallback(const std::function<void()>& callback)
{
	onDeactivatedCallback_ = callback;
}
