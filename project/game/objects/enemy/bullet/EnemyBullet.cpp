#include "EnemyBullet.h"

#include "DeltaTimer.h"

#include "objects/enemy/adjustItem/EnemyAdjustItem.h"

void EnemyBullet::Finalize()
{
	Object3d::RemoveRenderer();
	Collider::RemoveCollider();
}

void EnemyBullet::Init(const std::string& colliderName, EnemyType type)
{
	Object3d::Initialize("Box.obj");
	Object3d::SetSceneRenderer();
	Object3d::GetMaterial().enableDraw = false;
	transform_.scale_ = { 0.1f,0.1f ,0.3f };
	transform_.translation_.y = -5.0f;

	isActive_ = false;
	activeFrame_ = 0.0f;
	type_ = type;

	Collider::AddCollider();
	Collider::myType_ = ColliderType::OBB;
	Collider::colliderName_ = colliderName;
	Collider::size_ = transform_.scale_;
	Collider::isActive_ = false;
	Collider::targetColliderName_ = {
		"Player","Building","DeadTree","fence","Bush","StoneWall","ShortStoneWall"
	};
	Collider::DrawCollider();
}

void EnemyBullet::Update()
{
	// フレームが60立ったらIsActiveをfalseにする
	if (isActive_) {
		activeFrame_ += DeltaTimer::GetDeltaTime() * 2.0f;
		if (activeFrame_ >= 1.0f) {
			activeFrame_ = 1.0f;
			isActive_ = false;
			Collider::isActive_ = false;
			Object3d::GetMaterial().enableDraw = false;
			Object3d::GetTransform().translation_ = -5.0f;
		}
	}

	// コールバック関数
	if (wasActive_ && !isActive_ && onDeactivatedCallback_) {
		onDeactivatedCallback_();
		Object3d::GetMaterial().enableDraw = false;
	}
	wasActive_ = isActive_;

	// Activeがfalseならこの先を更新しない
	if (!isActive_) {
		Collider::Update();
		Object3d::Update();
		return;
	}

	// 移動処理
	float speed = GetTypeBulletSpeed();
	transform_.translation_ += velocity_ * DeltaTimer::GetDeltaTime() * speed;

	Collider::rotate_ = transform_.rotation_;
	Collider::centerPosition_ = transform_.translation_;
	Collider::Update();
	Object3d::Update();
}

void EnemyBullet::OnCollisionEnter(Collider* other)
{
	const auto& name = other->GetColliderName();

	// 当たったらな消す
	if (name == "Player" || name == "DeadTree" ||
		name == "Building" || name == "fence" || name == "Bush" ||
		name == "StoneWall" || name == "ShortStoneWall") {
		IsCollision();
	}
}

void EnemyBullet::Attack(const WorldTransform& transform)
{
	// 回転と座標を取得
	Matrix4x4 rotateMatrix = Quaternion::MakeRotateMatrix(transform.rotation_);
	transform_.rotation_ = transform.rotation_;
	transform_.translation_ = transform.translation_ + Vector3{ 0.0f,1.0f,0.0f };

	// 速度(向き)を取得
	velocity_ = Vector3::ExprUnitZ.Transform(rotateMatrix);

	activeFrame_ = 0.0f; 
	isActive_ = true;
	Collider::isActive_ = true;
	Object3d::GetMaterial().enableDraw = true;
}

void EnemyBullet::IsCollision()
{
	activeFrame_ = 1.0f;
	wasActive_ = false;
	isActive_ = false;
	Collider::isActive_ = false;
	Object3d::GetMaterial().enableDraw = false;
}

void EnemyBullet::SetOnDeactivateCallback(const std::function<void()>& callback)
{
	onDeactivatedCallback_ = callback;
}

const float EnemyBullet::GetTypeBulletSpeed()
{
	switch (type_)
	{
	case EnemyType::Ranged:			return item_->GetRangedData().bulletSpeed;
	case EnemyType::RangedElite:	return item_->GetRangedEliteData().bulletSpeed;
	default:
		break;
	}
	return 0.0f;
}
