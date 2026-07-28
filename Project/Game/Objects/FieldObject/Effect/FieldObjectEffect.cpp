#include "FieldObjectEffect.h"

void FieldObjectEffect::Init(const std::string& name)
{
	// ヒット時のエフェクト
    if (name == "DeadTree") {
		hitEmitter_ = std::make_shared<NumaEngine::ParticleEmitter>("woodChip");
		particleManager_->CreateParticleGroup(hitEmitter_);
		hitEmitter_->SetIsCreate(false);

		breakEmitter_ = std::make_shared<NumaEngine::ParticleEmitter>("breakTree");
		particleManager_->CreateParticleGroup(breakEmitter_);
		breakEmitter_->SetIsCreate(false);
	}
	if (name == "fence") {
		hitEmitter_ = std::make_shared<NumaEngine::ParticleEmitter>("fenceChip");
		particleManager_->CreateParticleGroup(hitEmitter_);
		hitEmitter_->SetIsCreate(false);
	}
}

void FieldObjectEffect::Update()
{

}

void FieldObjectEffect::OnceHitEffect(const WorldTransform& transform)
{
	hitEmitter_->onceEmit();

	NumaEngine::Quaternion rotate = transform.rotation_;
	NumaEngine::Vector3 position = transform.translation_;

	hitEmitter_->SetRotation(rotate);
	hitEmitter_->SetPosition(position);
}

void FieldObjectEffect::OnceBreakEffect(const WorldTransform& transform)
{
	/*if (isBreakEffectPlay_) { return; }
	isBreakEffectPlay_ = true;*/
	breakEmitter_->onceEmit();

	NumaEngine::Quaternion rotate = transform.rotation_;
	NumaEngine::Vector3 position = transform.translation_;

	breakEmitter_->SetRotation(rotate);
	breakEmitter_->SetPosition(position);
}

