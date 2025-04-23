#include "PlayerEffect.h"

#include "gameScene/player/Player.h"

void PlayerEffect::Init()
{
	// 移動時のエフェクト
	moveDustEmitter_ = std::make_unique<ParticleEmitter>("moveDust");
	particleManager_->CreateParticleGroup("moveDust", "smoke.png", moveDustEmitter_.get());
	moveDustEmitter_->SetIsCreate(false);

	// 弾のエフェクト
	for (size_t i = 0; i < bulletTrailEmitters_.size(); ++i) {
		bulletTrailEmitters_[i] = std::make_unique<ParticleEmitter>("bulletTrail");
		particleManager_->CreateParticleGroup("bulletTrail" + std::to_string(i), "white1x1.png", bulletTrailEmitters_[i].get());
		bulletTrailEmitters_[i]->SetIsCreate(false);

		bulletHitEmitters_[i] = std::make_unique<ParticleEmitter>("bulletHit");
		particleManager_->CreateParticleGroup("bulletHit" + std::to_string(i), "white1x1.png", bulletHitEmitters_[i].get());
		bulletHitEmitters_[i]->SetIsCreate(false);

		bulletDeleteEmitters_[i] = std::make_unique<ParticleEmitter>("bulletDelete");
		particleManager_->CreateParticleGroup("bulletDelete" + std::to_string(i), "white1x1.png", bulletDeleteEmitters_[i].get());
		bulletDeleteEmitters_[i]->SetIsCreate(false);
	}

	// 弾を撃つ時のエフェクト
	bulletExplosionEmitter_ = std::make_unique<ParticleEmitter>("bulletExplosion");
	particleManager_->CreateParticleGroup("bulletExplosion", "circle.png", bulletExplosionEmitter_.get());
	bulletExplosionEmitter_->SetIsCreate(false);

	bulletSparkEmitter_ = std::make_unique<ParticleEmitter>("bulletSpark");
	particleManager_->CreateParticleGroup("bulletSpark", "circle.png", bulletSparkEmitter_.get());
	bulletSparkEmitter_->SetIsCreate(false);

	bulletSmokeEmitter_ = std::make_unique<ParticleEmitter>("bulletSmoke");
	particleManager_->CreateParticleGroup("bulletSmoke", "smoke.png", bulletSmokeEmitter_.get());
	bulletSmokeEmitter_->SetIsCreate(false);

	bulletCartridgeEmitter_ = std::make_unique<ParticleEmitter>("bulletCartridge");
	particleManager_->CreateParticleGroup("bulletCartridge", "white1x1.png", bulletCartridgeEmitter_.get());
	bulletCartridgeEmitter_->SetIsCreate(false);

	// 避け時のエフェクト
	avoidDustEmitter_ = std::make_unique<ParticleEmitter>("avoidDust");
	particleManager_->CreateParticleGroup("avoidDust", "smoke.png", avoidDustEmitter_.get());
	avoidDustEmitter_->SetIsCreate(false);
}

void PlayerEffect::Update()
{	
}

void PlayerEffect::Draw()
{
}

void PlayerEffect::OnceMoveEffect()
{
	moveDustEmitter_->onceEmit();

	// パーティクルの座標を設定
	Quaternion rotate = player_->GetTransform().rotation_;
	Vector3 position = player_->GetTransform().translation_;
	
	moveDustEmitter_->SetRotation(rotate);
	moveDustEmitter_->SetPosition(position);
}

void PlayerEffect::OnceBulletTrailEffect(const int32_t count, const WorldTransform& transform)
{
	bulletTrailEmitters_[count]->onceEmit();

	// パーティクルの座標を設定
	Quaternion rotate = transform.rotation_;
	Vector3 position = transform.translation_;

	bulletTrailEmitters_[count]->SetRotation(rotate);
	bulletTrailEmitters_[count]->SetPosition(position);
}

void PlayerEffect::OnceBulletHitEffect(const int32_t count, const WorldTransform& transform)
{
	bulletHitEmitters_[count]->onceEmit();

	// パーティクルの座標を設定
	Quaternion rotate = transform.rotation_;
	Vector3 position = transform.translation_;

	bulletHitEmitters_[count]->SetRotation(rotate);
	bulletHitEmitters_[count]->SetPosition(position);
}

void PlayerEffect::OnceBulletDeleteEffect(const int32_t count, const WorldTransform& transform)
{
	bulletDeleteEmitters_[count]->onceEmit();

	// パーティクルの座標を設定
	Quaternion rotate = transform.rotation_;
	Vector3 position = transform.translation_;

	bulletDeleteEmitters_[count]->SetRotation(rotate);
	bulletDeleteEmitters_[count]->SetPosition(position);
}

void PlayerEffect::OnceBulletEffect()
{
	// Particleを一回生成
	bulletExplosionEmitter_->onceEmit();
	bulletSparkEmitter_->onceEmit();
	bulletSmokeEmitter_->onceEmit();
	bulletCartridgeEmitter_->onceEmit();

	// パーティクルの座標を設定
	Quaternion rotate = player_->GetTransform().rotation_;
	Vector3 position = player_->GetTransform().translation_;

	// 爆発
	bulletExplosionEmitter_->SetPosition(position);
	bulletExplosionEmitter_->SetRotation(rotate);
	// 火花
	bulletSparkEmitter_->SetPosition(position);
	bulletSparkEmitter_->SetRotation(rotate);
	// 煙
	bulletSmokeEmitter_->SetPosition(position);
	bulletSmokeEmitter_->SetRotation(rotate);
	// 薬莢
	bulletCartridgeEmitter_->SetPosition(position);
	bulletCartridgeEmitter_->SetRotation(rotate);
}

void PlayerEffect::OnceAvoidEffect()
{
	avoidDustEmitter_->onceEmit();

	// パーティクルの座標を設定
	Quaternion rotate = player_->GetTransform().rotation_;
	Vector3 position = player_->GetTransform().translation_;

	avoidDustEmitter_->SetPosition(position);
	avoidDustEmitter_->SetRotation(rotate);
}
