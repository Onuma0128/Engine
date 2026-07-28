#include "EnemyBullet.h"

#include "DeltaTimer.h"

#include "Collision/CollisionFilter.h"
#include "Objects/Enemy/AdjustItem/EnemyAdjustItem.h"

void EnemyBullet::Init(const std::string& colliderName, EnemyType type)
{
	Object3d::Initialize("Box.obj");
	Object3d::SetSceneRenderer();
	Object3d::GetMaterial().enableDraw = false;
	transform_.scale_ = { 0.1f,0.1f ,0.3f };
	transform_.translation_ = item_->GetMainData().startPosition;

	isActive_ = false;
	activeFrame_ = 0.0f;
	type_ = type;

    NumaEngine::Collider::AddCollider();
	NumaEngine::Collider::myType_ = NumaEngine::ColliderType::kOBB;
	NumaEngine::Collider::colliderName_ = colliderName;
	NumaEngine::Collider::size_ = transform_.scale_;
	NumaEngine::Collider::isActive_ = false;
	NumaEngine::Collider::targetColliderName_ = {
		"Player","Building","DeadTree","fence","Bush","StoneWall","ShortStoneWall",
		"MuscleCompanion","PushUpMuscleCompanion",
	};
	Collider::DrawCollider();
}

void EnemyBullet::Update()
{
	// フレームが60立ったらIsActiveをfalseにする
	if (isActive_) {
        activeFrame_ += NumaEngine::DeltaTimer::GetDeltaTime() * 2.0f;
		if (activeFrame_ >= 1.0f) {
			activeFrame_ = 1.0f;
            isActive_ = false;
			NumaEngine::Collider::isActive_ = false;
			Object3d::GetMaterial().enableDraw = false;
			transform_.translation_ = item_->GetMainData().startPosition;		}
	}

	// コールバック関数
	if (wasActive_ && !isActive_ && onDeactivatedCallback_) {
		onDeactivatedCallback_();
		Object3d::GetMaterial().enableDraw = false;
	}
	wasActive_ = isActive_;

	// Activeがfalseならこの先を更新しない
	if (!isActive_) {
        NumaEngine::Collider::Update();
		Object3d::Update();
		return;
	}

	// 移動処理
	float speed = GetTypeBulletSpeed();
    transform_.translation_ += velocity_ * NumaEngine::DeltaTimer::GetDeltaTime() * speed;

    NumaEngine::Collider::rotate_ = transform_.rotation_;
	NumaEngine::Collider::centerPosition_ = transform_.translation_;
	NumaEngine::Collider::Update();
	Object3d::Update();
}

void EnemyBullet::OnCollisionEnter(NumaEngine::Collider* other)
{
	const auto& name = other->GetColliderName();

	// 当たったらな消す
	if (CollisionFilter::CheckColliderNameFieldObject(other->GetColliderName()) ||
		other->GetColliderName() == "MuscleCompanion" ||
		other->GetColliderName() == "PushUpMuscleCompanion") {
		IsCollision();
	}
}

void EnemyBullet::Attack(const NumaEngine::WorldTransform& transform)
{
	// 回転と座標を取得
    NumaEngine::Matrix4x4 rotateMatrix = NumaEngine::Quaternion::MakeRotateMatrix(transform.rotation_);
	transform_.rotation_ = transform.rotation_;
	transform_.translation_ = transform.translation_ + NumaEngine::Vector3::ExprUnitY;

	// 速度(向き)を取得
	velocity_ = NumaEngine::Vector3::ExprUnitZ.Transform(rotateMatrix);

	activeFrame_ = 0.0f; 
    isActive_ = true;
	NumaEngine::Collider::isActive_ = true;
	Object3d::GetMaterial().enableDraw = true;
}

void EnemyBullet::IsCollision()
{
	activeFrame_ = 1.0f;
    wasActive_ = false;
	isActive_ = false;
	NumaEngine::Collider::isActive_ = false;
	transform_.translation_ = item_->GetMainData().startPosition;
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
	case EnemyType::kRanged:		return item_->GetRangedData().bulletSpeed;
	case EnemyType::kRangedElite:	return item_->GetRangedEliteData().bulletSpeed;
	default:
		break;
	}
	return 0.0f;
}

