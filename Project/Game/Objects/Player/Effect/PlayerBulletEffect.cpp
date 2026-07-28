#include "PlayerBulletEffect.h"

void PlayerBulletEffect::Init()
{
	// 弾のエフェクト
    bulletTrailEmitter_ = std::make_shared<NumaEngine::ParticleEmitter>("bulletTrail");
	particleManager_->CreateParticleGroup(bulletTrailEmitter_);
	bulletTrailEmitter_->SetIsCreate(false);

	bulletDeleteEmitter_ = std::make_shared<NumaEngine::ParticleEmitter>("bulletDelete");
	particleManager_->CreateParticleGroup(bulletDeleteEmitter_);
	bulletDeleteEmitter_->SetIsCreate(false);

	// 弾を撃つ時のエフェクト
	bulletExplosionEmitter_ = std::make_shared<NumaEngine::ParticleEmitter>("bulletExplosion");
	particleManager_->CreateParticleGroup(bulletExplosionEmitter_);
	bulletExplosionEmitter_->SetIsCreate(false);

	bulletSparkEmitter_ = std::make_shared<NumaEngine::ParticleEmitter>("bulletSpark");
	particleManager_->CreateParticleGroup(bulletSparkEmitter_);
	bulletSparkEmitter_->SetIsCreate(false);

	bulletSmokeEmitter_ = std::make_shared<NumaEngine::ParticleEmitter>("bulletSmoke");
	particleManager_->CreateParticleGroup(bulletSmokeEmitter_);
	bulletSmokeEmitter_->SetIsCreate(false);

	bulletCartridgeEmitter_ = std::make_shared<NumaEngine::ParticleEmitter>("bulletCartridge");
	particleManager_->CreateParticleGroup(bulletCartridgeEmitter_);
	bulletCartridgeEmitter_->SetIsCreate(false);
}

void PlayerBulletEffect::Update()
{
	bulletTrailEmitter_->SetIsCreate(false);
}

void PlayerBulletEffect::OnceBulletTrailEffect(const NumaEngine::WorldTransform& transform)
{
	bulletTrailEmitter_->SetIsCreate(true);

	// パーティクルの座標を設定
	NumaEngine::Quaternion rotate = transform.rotation_;
	NumaEngine::Vector3 position = transform.translation_;

	bulletTrailEmitter_->SetRotation(rotate);
	bulletTrailEmitter_->SetPosition(position);
}

void PlayerBulletEffect::OnceBulletDeleteEffect(const NumaEngine::WorldTransform& transform)
{
	bulletDeleteEmitter_->onceEmit();

	// パーティクルの座標を設定
	NumaEngine::Quaternion rotate = transform.rotation_;
	NumaEngine::Vector3 position = transform.translation_;

	bulletDeleteEmitter_->SetRotation(rotate);
	bulletDeleteEmitter_->SetPosition(position);
}

void PlayerBulletEffect::OnceBulletEffect(const NumaEngine::WorldTransform& transform)
{
	// Particleを一回生成
	bulletExplosionEmitter_->onceEmit();
	bulletSparkEmitter_->onceEmit();
	bulletSmokeEmitter_->onceEmit();

	// パーティクルの座標を設定
	NumaEngine::Quaternion rotate = transform.rotation_;
	NumaEngine::Vector3 position = transform.translation_;

	// 爆発
	bulletExplosionEmitter_->SetPosition(position);
	bulletExplosionEmitter_->SetRotation(rotate);
	// 火花
	bulletSparkEmitter_->SetPosition(position);
	bulletSparkEmitter_->SetRotation(rotate);
	// 煙
	bulletSmokeEmitter_->SetPosition(position);
	bulletSmokeEmitter_->SetRotation(rotate);
}

void PlayerBulletEffect::OnceBulletReloadEffect(const NumaEngine::WorldTransform& transform)
{
	// Particleを一回生成
	bulletCartridgeEmitter_->onceEmit();

	// パーティクルの座標を設定
	NumaEngine::Quaternion rotate = transform.rotation_;
	NumaEngine::Vector3 position = transform.translation_;

	// 薬莢
	bulletCartridgeEmitter_->SetPosition(position);
	bulletCartridgeEmitter_->SetRotation(rotate);
}


