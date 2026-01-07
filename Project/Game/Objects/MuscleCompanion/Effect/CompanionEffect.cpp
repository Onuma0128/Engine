#include "CompanionEffect.h"

#include "Objects/MuscleCompanion/Base/MuscleCompanion.h"

void CompanionEffect::Init()
{
	// 移動時のエフェクト
	moveDustEmitter_ = std::make_unique<ParticleEmitter>("moveDust");
	particleManager_->CreateParticleGroup(moveDustEmitter_);
	moveDustEmitter_->SetIsCreate(false);

	// 移動時のエフェクト
	dashEmitter_ = std::make_unique<ParticleEmitter>("dash");
	particleManager_->CreateParticleGroup(dashEmitter_);
	dashEmitter_->SetIsCreate(false);

	// ヒット時のエフェクト
	hitEmitter_ = std::make_unique<ParticleEmitter>("hitDamage");
	particleManager_->CreateParticleGroup(hitEmitter_);
	hitEmitter_->SetIsCreate(false);
}

void CompanionEffect::Update()
{

}

void CompanionEffect::Draw()
{

}

void CompanionEffect::OnceMoveEffect()
{
	moveDustEmitter_->onceEmit();

	// パーティクルの座標を設定
	Quaternion rotate = companion_->GetTransform().rotation_;
	Vector3 position = companion_->GetTransform().translation_;

	moveDustEmitter_->SetRotation(rotate);
	moveDustEmitter_->SetPosition(position);
}

void CompanionEffect::OnceDashEffect()
{
	dashEmitter_->onceEmit();

	// パーティクルの座標を設定
	Quaternion rotate = companion_->GetTransform().rotation_;
	Vector3 position = companion_->GetTransform().translation_;

	dashEmitter_->SetRotation(rotate);
	dashEmitter_->SetPosition(position);
}

void CompanionEffect::OnceHitEffect()
{
	hitEmitter_->onceEmit();

	// パーティクルの座標を設定
	Quaternion rotate = companion_->GetTransform().rotation_;
	Vector3 position = companion_->GetTransform().translation_;

	hitEmitter_->SetRotation(rotate);
	hitEmitter_->SetPosition(position);
}