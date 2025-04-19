#include "PlayerEffect.h"

#include "gameScene/player/Player.h"

void PlayerEffect::Init()
{
	// 移動時のエフェクト
	moveDustEmitter_ = std::make_unique<ParticleEmitter>("moveDust");
	particleManager_->CreateParticleGroup("moveDust", "smoke.png", moveDustEmitter_.get());
	moveDustEmitter_->SetIsCreate(false);

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
	Vector3 position = player_->GetTransform().translation_;
	Vector3 offset = Vector3{ 0.0f,-0.4f,-0.5f }.Transform(Quaternion::MakeRotateMatrix(player_->GetTransform().rotation_));

	moveDustEmitter_->SetRotation(player_->GetTransform().rotation_);
	moveDustEmitter_->SetPosition(position + offset);
}

void PlayerEffect::OnceBulletEffect()
{
	// Particleを一回生成
	bulletExplosionEmitter_->onceEmit();
	bulletSparkEmitter_->onceEmit();
	bulletSmokeEmitter_->onceEmit();
	bulletCartridgeEmitter_->onceEmit();

	// パーティクルの座標を設定
	Vector3 position = player_->GetTransform().translation_;
	Vector3 offset = Vector3::ExprUnitZ.Transform(Quaternion::MakeRotateMatrix(player_->GetTransform().rotation_));

	// 爆発
	bulletExplosionEmitter_->SetPosition(position + offset);
	// 火花
	bulletSparkEmitter_->SetPosition(position + offset);
	bulletSparkEmitter_->SetRotation(player_->GetTransform().rotation_);
	// 煙
	bulletSmokeEmitter_->SetPosition(position + offset + Vector3{ 0.0f,0.2f,0.0f });
	// 薬莢
	bulletCartridgeEmitter_->SetPosition(position + offset);
	bulletCartridgeEmitter_->SetRotation(player_->GetTransform().rotation_);
}

void PlayerEffect::OnceAvoidEffect()
{
	avoidDustEmitter_->onceEmit();

	// パーティクルの座標を設定
	Vector3 position = player_->GetTransform().translation_;
	Vector3 offset = Vector3{ 0.0f,-0.4f,0.0f }.Transform(Quaternion::MakeRotateMatrix(player_->GetTransform().rotation_));

	avoidDustEmitter_->SetPosition(position + offset);
	avoidDustEmitter_->SetRotation(player_->GetTransform().rotation_);
}
