#include "CompanionAttackCollider.h"

#include "Objects/MuscleCompanion/Base/MuscleCompanion.h"
#include "GameCamera/GameCamera.h"

void CompanionAttackCollider::Initialize()
{
    // コライダーを設定
	NumaEngine::Collider::AddCollider();
	NumaEngine::Collider::myType_ = NumaEngine::ColliderType::kSphere;
	NumaEngine::Collider::colliderName_ = "MuscleCompanionAttack";
	NumaEngine::Collider::isActive_ = false;
	NumaEngine::Collider::targetColliderName_ = { "Enemy","BossEnemy"};
	NumaEngine::Collider::DrawCollider();
}

void CompanionAttackCollider::Update()
{
	isHit_ = false;
    NumaEngine::Collider::radius_ = companion_->GetItems()->GetAttackData().attackColliderSize;
	NumaEngine::Collider::rotate_ = companion_->GetTransform().rotation_;
	NumaEngine::Vector3 offset = companion_->GetItems()->GetAttackData().attackColliderOffset.Transform(
		NumaEngine::Quaternion::MakeRotateMatrix(companion_->GetTransform().rotation_));
    NumaEngine::Collider::centerPosition_ = companion_->GetTransform().translation_ + offset;
	NumaEngine::Collider::Update();
}

void CompanionAttackCollider::OnCollisionEnter(NumaEngine::Collider* other)
{
	if (!isHit_) {
		const auto& volume = companion_->GetItems()->GetSeVolumeData();
		companion_->GetAudio()->SoundPlayWave("MattyoGiveDamage.wav", volume.giveDamage);
		companion_->GetCamera()->SetShake(companion_->GetItems()->GetAttackData().shakePowerLow);
	}
	isHit_ = true;
	hitColliders_.push_back(other);
}

void CompanionAttackCollider::OnCollisionStay(NumaEngine::Collider* other)
{
}

void CompanionAttackCollider::OnCollisionExit(NumaEngine::Collider* other)
{
}

