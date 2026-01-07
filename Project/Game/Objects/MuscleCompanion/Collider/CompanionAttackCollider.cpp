#include "CompanionAttackCollider.h"

#include "Objects/MuscleCompanion/Base/MuscleCompanion.h"

void CompanionAttackCollider::Initialize()
{
	// コライダーを設定
	Collider::AddCollider();
	Collider::myType_ = ColliderType::kSphere;
	Collider::colliderName_ = "MuscleCompanionAttack";
	Collider::isActive_ = false;
	Collider::targetColliderName_ = { "Enemy","BossEnemy"};
	Collider::DrawCollider();
}

void CompanionAttackCollider::Update()
{
	isHit_ = false;
	Collider::radius_ = companion_->GetItems()->GetAttackData().attackColliderSize;
	Collider::rotate_ = companion_->GetTransform().rotation_;
	Vector3 offset = companion_->GetItems()->GetAttackData().attackColliderOffset.Transform(
		Quaternion::MakeRotateMatrix(companion_->GetTransform().rotation_));
	Collider::centerPosition_ = companion_->GetTransform().translation_ + offset;
	Collider::Update();
}

void CompanionAttackCollider::OnCollisionEnter(Collider* other)
{
	isHit_ = true;
	hitColliders_.push_back(other);

	if (!companion_->GetAudio()->IsPlaying("MattyoGiveDamage.wav")) {
		const auto& volume = companion_->GetItems()->GetSeVolumeData();
		companion_->GetAudio()->SoundPlayWave("MattyoGiveDamage.wav", volume.giveDamage);
	}
}

void CompanionAttackCollider::OnCollisionStay(Collider* other)
{
}

void CompanionAttackCollider::OnCollisionExit(Collider* other)
{
}
