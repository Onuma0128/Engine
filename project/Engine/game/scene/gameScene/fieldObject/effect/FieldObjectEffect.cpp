#include "FieldObjectEffect.h"

void FieldObjectEffect::Init()
{
	// 移動時のエフェクト
	woodChipEmitter_ = std::make_unique<ParticleEmitter>("woodChip");
	particleManager_->CreateParticleGroup(woodChipEmitter_);
	woodChipEmitter_->SetIsCreate(false);
}

void FieldObjectEffect::Update()
{

}

void FieldObjectEffect::OnceWoodChipEffect(const WorldTransform& transform)
{
	woodChipEmitter_->onceEmit();

	Quaternion rotate = transform.rotation_;
	Vector3 position = transform.translation_;

	woodChipEmitter_->SetRotation(rotate);
	woodChipEmitter_->SetPosition(position);
}
