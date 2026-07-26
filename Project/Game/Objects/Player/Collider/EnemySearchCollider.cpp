#include "EnemySearchCollider.h"

#include "Objects/Player/Player.h"
#include "Objects/Player/AdjustItem/PlayerAdjustItem.h"

void EnemySearchCollider::Initialize()
{
    NumaEngine::Collider::AddCollider();
	NumaEngine::Collider::colliderName_ = "EnemySearch";
	NumaEngine::Collider::myType_ = NumaEngine::ColliderType::kSphere;
	NumaEngine::Collider::targetColliderName_ = { "Enemy" };
	NumaEngine::Collider::DrawCollider();
}

void EnemySearchCollider::Update()
{
	ResetTarget();

	NumaEngine::Collider::radius_ = player_->GetItem()->GetPlayerData().searchRadius;
	NumaEngine::Collider::centerPosition_ = player_->GetTransform().translation_;
	NumaEngine::Collider::Update();

	isHit_ = false;
}

void EnemySearchCollider::OnCollisionEnter(NumaEngine::Collider* other)
{
}

void EnemySearchCollider::OnCollisionStay(NumaEngine::Collider* other)
{
    // 距離を計算する
	float distance = NumaEngine::Vector3::Distance(this->centerPosition_, other->GetCenterPosition());

	// 一番近い敵の座標を更新する
	if(distance < targetDistance_) {
		targetDistance_ = distance;
		targetPosition_ = other->GetCenterPosition();
		targetCollider_ = other;
	}

	isHit_ = true;
}

void EnemySearchCollider::OnCollisionExit(NumaEngine::Collider* other)
{
}

void EnemySearchCollider::ResetTarget()
{
	targetDistance_ = FLT_MAX;
	targetPosition_ = NumaEngine::Vector3::ExprZero;
	targetCollider_ = nullptr;
}

