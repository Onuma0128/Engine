#include "BossAttackCollider.h"

#include "Objects/Boss/Base/BossEnemy.h"

void BossAttackCollider::Initialize()
{
    // コライダーを設定
	NumaEngine::Collider::AddCollider();
	NumaEngine::Collider::myType_ = NumaEngine::ColliderType::kSphere;
	NumaEngine::Collider::colliderName_ = "BossAttack";
	NumaEngine::Collider::isActive_ = false;
	NumaEngine::Collider::targetColliderName_ = { "Player","MuscleCompanion","SearchDashMuscleCompanion", };
	NumaEngine::Collider::DrawCollider();
}

void BossAttackCollider::Update()
{
    NumaEngine::Collider::radius_ = colliderSize_;
	NumaEngine::Collider::centerPosition_ = boss_->GetTransform().translation_ + colliderOffset_;
	NumaEngine::Collider::Update();
}

void BossAttackCollider::OnCollisionEnter(NumaEngine::Collider* other)
{
}

void BossAttackCollider::OnCollisionStay(NumaEngine::Collider* other)
{
}

void BossAttackCollider::OnCollisionExit(NumaEngine::Collider* other)
{
}
