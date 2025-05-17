#include "EnemyEffect.h"

#include "gameScene/enemy/Enemy.h"

void EnemyEffect::Init()
{
	// ヒット時のエフェクト
	hitEmitter_ = std::make_unique<ParticleEmitter>("enemyHit");
	particleManager_->CreateParticleGroup("enemyHit", "white1x1.png", hitEmitter_.get());
	hitEmitter_->SetIsCreate(false);

	hitDustEmitter_ = std::make_unique<ParticleEmitter>("enemyHitExplosion");
	particleManager_->CreateParticleGroup("enemyHitExplosion", "circle.png", hitDustEmitter_.get());
	hitDustEmitter_->SetIsCreate(false);

}

void EnemyEffect::Update()
{
}

void EnemyEffect::OnceBulletHitEffect(const WorldTransform& transform)
{
	hitEmitter_->onceEmit();
	hitDustEmitter_->onceEmit();

	// パーティクルの座標を設定
	Quaternion rotate = transform.rotation_;
	Vector3 position = transform.translation_;

	hitEmitter_->SetRotation(rotate);
	hitEmitter_->SetPosition(position);
	hitDustEmitter_->SetRotation(rotate);
	hitDustEmitter_->SetPosition(position);
}
