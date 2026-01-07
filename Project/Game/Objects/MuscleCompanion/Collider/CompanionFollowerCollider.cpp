#include "CompanionFollowerCollider.h"

#include "Objects/MuscleCompanion/Base/MuscleCompanion.h"

void CompanionFollowerCollider::Initialize()
{
	// コライダーを設定
	Collider::AddCollider();
	Collider::myType_ = ColliderType::kSphere;
	Collider::colliderName_ = "FollowerMuscleCompanion";
	Collider::isActive_ = false;
	Collider::targetColliderName_ = { "Enemy" };
	Collider::DrawCollider();
}

void CompanionFollowerCollider::Update()
{
	Collider::radius_ = companion_->GetItems()->GetMainData().followerColliderSize;
	Collider::centerPosition_ = companion_->GetTransform().translation_ + companion_->GetItems()->GetMainData().colliderOffset;
	Collider::Update();
}

void CompanionFollowerCollider::OnCollisionEnter(Collider* other)
{
	const auto& volume = companion_->GetItems()->GetSeVolumeData();
	companion_->GetAudio()->SoundPlayWave("MattyoGiveDamage.wav", volume.giveDamage);
}

void CompanionFollowerCollider::OnCollisionStay(Collider* other)
{
}

void CompanionFollowerCollider::OnCollisionExit(Collider* other)
{
}
