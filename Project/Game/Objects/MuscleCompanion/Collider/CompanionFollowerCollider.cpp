#include "CompanionFollowerCollider.h"

#include "Objects/MuscleCompanion/Base/MuscleCompanion.h"
#include "GameCamera/GameCamera.h"

void CompanionFollowerCollider::Initialize()
{
    // コライダーを設定
	NumaEngine::Collider::AddCollider();
	NumaEngine::Collider::myType_ = NumaEngine::ColliderType::kSphere;
	NumaEngine::Collider::colliderName_ = "FollowerMuscleCompanion";
	NumaEngine::Collider::isActive_ = false;
	NumaEngine::Collider::targetColliderName_ = { "Enemy" };
	NumaEngine::Collider::DrawCollider();
}

void CompanionFollowerCollider::Update()
{
    NumaEngine::Collider::radius_ = companion_->GetItems()->GetMainData().followerColliderSize * companion_->GetTransform().scale_.x;
	NumaEngine::Collider::centerPosition_ = companion_->GetTransform().translation_ + companion_->GetItems()->GetMainData().colliderOffset;
	NumaEngine::Collider::Update();
}

void CompanionFollowerCollider::OnCollisionEnter(NumaEngine::Collider* other)
{
	const auto& volume = companion_->GetItems()->GetSeVolumeData();
	companion_->GetAudio()->SoundPlayWave("MattyoGiveDamage.wav", volume.giveDamage);
	companion_->GetCamera()->SetShake(companion_->GetItems()->GetAttackData().shakePowerLow);
}

void CompanionFollowerCollider::OnCollisionStay(NumaEngine::Collider* other)
{
}

void CompanionFollowerCollider::OnCollisionExit(NumaEngine::Collider* other)
{
}
