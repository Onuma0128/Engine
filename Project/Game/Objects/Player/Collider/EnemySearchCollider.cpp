#include "EnemySearchCollider.h"

#include "Objects/Player/Player.h"
#include "Objects/Player/AdjustItem/PlayerAdjustItem.h"

void EnemySearchCollider::Initialize()
{
	Collider::AddCollider();
	Collider::colliderName_ = "EnemySearch";
	Collider::myType_ = ColliderType::kSphere;
	Collider::targetColliderName_ = { "Enemy" };
	Collider::DrawCollider();
}

void EnemySearchCollider::Update()
{
	ResetTarget();

	Collider::radius_ = player_->GetItem()->GetPlayerData().searchRadius;
	Collider::centerPosition_ = player_->GetTransform().translation_;
	Collider::Update();

	isHit_ = false;
}

void EnemySearchCollider::OnCollisionEnter(Collider* other)
{
}

void EnemySearchCollider::OnCollisionStay(Collider* other)
{
	// 距離を計算する
	float distance = Vector3::Distance(Collider::centerPosition_, other->GetCenterPosition());

	// 一番近い敵の座標を更新する
	if(distance < targetDistance_) {
		targetDistance_ = distance;
		targetPosition_ = other->GetCenterPosition();
		targetCollider_ = other;
	}

	isHit_ = true;
}

void EnemySearchCollider::OnCollisionExit(Collider* other)
{
}

void EnemySearchCollider::ResetTarget()
{
	targetDistance_ = FLT_MAX;
	targetPosition_ = Vector3::ExprZero;
	targetCollider_ = nullptr;
}
