#include "FieldObjectEffect.h"

void FieldObjectEffect::Init(const std::string& name)
{
	// ヒット時のエフェクト
	if (name == "DeadTree") {
		hitEmitter_ = std::make_unique<ParticleEmitter>("woodChip");
		particleManager_->CreateParticleGroup(hitEmitter_);
		hitEmitter_->SetIsCreate(false);
	}
	if (name == "fence") {
		hitEmitter_ = std::make_unique<ParticleEmitter>("fenceChip");
		particleManager_->CreateParticleGroup(hitEmitter_);
		hitEmitter_->SetIsCreate(false);
	}
}

void FieldObjectEffect::Update()
{

}

void FieldObjectEffect::OnceWoodChipEffect(const WorldTransform& transform)
{
	hitEmitter_->onceEmit();

	Quaternion rotate = transform.rotation_;
	Vector3 position = transform.translation_;

	hitEmitter_->SetRotation(rotate);
	hitEmitter_->SetPosition(position);
}
