#include "BossAttackCollider.h"

#include "Objects/Boss/Base/BossEnemy.h"

void BossAttackCollider::Initialize()
{
	// コライダーを設定
	Collider::AddCollider();
	Collider::myType_ = ColliderType::kSphere;
	Collider::colliderName_ = "BossAttack";
	Collider::isActive_ = false;
	Collider::targetColliderName_ = { "Player","MuscleCompanion" };
	Collider::DrawCollider();
}

void BossAttackCollider::Update()
{
	Collider::radius_ = colliderSize_;
	Collider::centerPosition_ = boss_->GetTransform().translation_ + colliderOffset_;
	Collider::Update();
}

void BossAttackCollider::OnCollisionEnter(Collider* other)
{
}

void BossAttackCollider::OnCollisionStay(Collider* other)
{
}

void BossAttackCollider::OnCollisionExit(Collider* other)
{
}
