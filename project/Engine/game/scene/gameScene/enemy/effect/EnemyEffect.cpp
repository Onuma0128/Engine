#include "EnemyEffect.h"

#include "DeltaTimer.h"
#include "Easing.h"

#include "gameScene/enemy/Enemy.h"

void EnemyEffect::Init()
{
	hitReticleEffect_.cylinder_ = std::make_unique<PrimitiveDrawr>();
	hitReticleEffect_.cylinder_->TypeInit(PrimitiveType::Cylinder, 32);
	hitReticleEffect_.cylinder_->GetTransform().scale = {};
	hitReticleEffect_.cylinder_->SetColor({ 1.0f,1.0f,0.0f });
	hitReticleEffect_.cylinder_->GetRenderOptions().enabled = false;
	hitReticleEffect_.cylinder_->GetRenderOptions().offscreen = false;
	hitReticleEffect_.frame_ = 0.0f;
	hitReticleEffect_.axis_ = 0.0f;

	// ヒット時のエフェクト
	hitEmitter_ = std::make_unique<ParticleEmitter>("enemyHit");
	particleManager_->CreateParticleGroup("enemyHit", "white1x1.png", hitEmitter_, true);
	hitEmitter_->SetIsCreate(false);

	hitExplosionEmitter_ = std::make_unique<ParticleEmitter>("enemyHitExplosion");
	particleManager_->CreateParticleGroup("enemyHitExplosion", "circle.png", hitExplosionEmitter_, true);
	hitExplosionEmitter_->SetIsCreate(false);

	// 死亡時のエフェクト
	deadEmitter_ = std::make_unique<ParticleEmitter>("enemyDead");
	particleManager_->CreateParticleGroup("enemyDead", "white1x1.png", deadEmitter_, true);
	deadEmitter_->SetIsCreate(false);
}

void EnemyEffect::Update()
{
	HitReticleUpdate();

	deadEmitter_->SetRotation(enemy_->GetTransform().rotation_);
	deadEmitter_->SetPosition(enemy_->GetTransform().translation_);
}

void EnemyEffect::Draw()
{
	if (hitReticleEffect_.cylinder_->GetRenderOptions().enabled) {
		hitReticleEffect_.cylinder_->TypeDraw();
	}
}

void EnemyEffect::OnceBulletHitEffect(const WorldTransform& transform)
{
	hitEmitter_->onceEmit();

	// パーティクルの座標を設定
	Quaternion rotate = transform.rotation_;
	Vector3 position = transform.translation_;

	hitEmitter_->SetRotation(rotate);
	hitEmitter_->SetPosition(position);
}

void EnemyEffect::OnceBulletHitExplosionEffect(const WorldTransform& transform)
{
	hitExplosionEmitter_->onceEmit();

	// パーティクルの座標を設定
	Quaternion rotate = transform.rotation_;
	Vector3 position = transform.translation_;

	hitExplosionEmitter_->SetRotation(rotate);
	hitExplosionEmitter_->SetPosition(position);
}

void EnemyEffect::HitReticleUpdate()
{
	float deltaTime = 1.0f / 60.0f;

	// ヒットしているなら描画をする
	if (enemy_->GetHitReticle()) {
		hitReticleEffect_.cylinder_->GetRenderOptions().enabled = true;
		hitReticleEffect_.frame_ += deltaTime;

	// ヒットが終わったら
	} else {
		hitReticleEffect_.frame_ -= deltaTime * 2.0f;

		// 描画を切る
		if (hitReticleEffect_.frame_ <= 0.0f) {
			hitReticleEffect_.cylinder_->GetRenderOptions().enabled = false;
		}
	}

	// Cylinderが描画されているなら
	if (hitReticleEffect_.cylinder_->GetRenderOptions().enabled) {
		// frameをクランプしてイージングを掛ける
		hitReticleEffect_.frame_ = std::clamp(hitReticleEffect_.frame_, 0.0f, 1.0f);
		float t = 0.0f;
		if(enemy_->GetHitReticle()){ t = Easing::EaseInQuint(hitReticleEffect_.frame_); }
		else { t = Easing::EaseOutBack(hitReticleEffect_.frame_); }
		float scale = t;
		// スケールと回転を適応
		hitReticleEffect_.cylinder_->GetTransform().scale = { scale,scale,scale };
		hitReticleEffect_.axis_ += 0.1f;
		hitReticleEffect_.cylinder_->GetTransform().rotation.y = hitReticleEffect_.axis_;
		Vector3 offset = { 0.0f,-0.5f,0.0f };
		hitReticleEffect_.cylinder_->GetTransform().translation = enemy_->GetTransform().translation_ + offset;
		hitReticleEffect_.cylinder_->Update();
	}
}
