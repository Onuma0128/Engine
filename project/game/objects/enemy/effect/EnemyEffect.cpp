#include "EnemyEffect.h"

#include "DeltaTimer.h"
#include "Easing.h"

#include "objects/enemy/base/BaseEnemy.h"
#include "objects/enemy/type/EnemyType.h"
#include "objects/enemy/adjustItem/EnemyAdjustItem.h"

void EnemyEffect::Init()
{

	HitReticleInit();

	BulletPredictionInit();

	// 移動時のエフェクト
	moveDustEmitter_ = std::make_unique<ParticleEmitter>("moveDust");
	particleManager_->CreateParticleGroup(moveDustEmitter_);
	moveDustEmitter_->SetIsCreate(false);

	// ヒット時のエフェクト
	hitEmitter_ = std::make_unique<ParticleEmitter>("enemyHit");
	particleManager_->CreateParticleGroup(hitEmitter_);
	hitEmitter_->SetIsCreate(false);

	hitExplosionEmitter_ = std::make_unique<ParticleEmitter>("enemyHitExplosion");
	particleManager_->CreateParticleGroup(hitExplosionEmitter_);
	hitExplosionEmitter_->SetIsCreate(false);

	hitRingEmitter_ = std::make_unique<ParticleEmitter>("enemyHitRing");
	particleManager_->CreateParticleGroup(hitRingEmitter_);
	hitRingEmitter_->SetIsCreate(false);

	// 死亡時のエフェクト
	deadEmitter_ = std::make_unique<ParticleEmitter>("enemyDead");
	particleManager_->CreateParticleGroup(deadEmitter_);
	deadEmitter_->SetIsCreate(false);

	enemyMeleeAttack_ = std::make_unique<ParticleEmitter>("enemyMeleeAttack");
	particleManager_->CreateParticleGroup(enemyMeleeAttack_);
	enemyMeleeAttack_->SetIsCreate(false);
}

void EnemyEffect::Update()
{
	HitReticleUpdate();

	BulletPredictionUpdate();

	//deadEmitter_->SetRotation(enemy_->GetTransform().rotation_);
	deadEmitter_->SetPosition(enemy_->GetTransform().translation_);
}

void EnemyEffect::Draw()
{
	if (hitReticleEffect_.cylinder_->GetRenderOptions().enabled) {
		hitReticleEffect_.cylinder_->TypeDraw();
	}
}

void EnemyEffect::OnceMoveEffect(const WorldTransform& transform)
{
	moveDustEmitter_->onceEmit();

	// パーティクルの座標を設定
	Quaternion rotate = transform.rotation_;
	Vector3 position = transform.translation_;

	moveDustEmitter_->SetRotation(rotate);
	moveDustEmitter_->SetPosition(position);
}

void EnemyEffect::OnceBulletEffect(const WorldTransform& transform)
{
	// Particleを一回生成
	bulletExplosionEmitter_->onceEmit();
	bulletSparkEmitter_->onceEmit();
	bulletSmokeEmitter_->onceEmit();

	// パーティクルの座標を設定
	Quaternion rotate = transform.rotation_;
	Vector3 position = transform.translation_;

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
	hitRingEmitter_->onceEmit();

	// パーティクルの座標を設定
	Quaternion rotate = transform.rotation_;
	Vector3 position = transform.translation_;

	hitExplosionEmitter_->SetRotation(rotate);
	hitExplosionEmitter_->SetPosition(position);
	hitRingEmitter_->SetRotation(rotate);
	hitRingEmitter_->SetPosition(position);
}

void EnemyEffect::SetMeleeAttackEffect(const WorldTransform& transform)
{
	// パーティクルの座標を設定
	Quaternion rotate = transform.rotation_;
	Vector3 position = transform.translation_;

	enemyMeleeAttack_->SetRotation(rotate);
	enemyMeleeAttack_->SetPosition(position);
}

void EnemyEffect::SetBulletPredictionEffect(bool flag)
{
	for (auto& effect : bulletPredictionEffect_) {
		effect.plane_->GetRenderOptions().enabled = flag;
	}
}

void EnemyEffect::HitReticleInit()
{
	hitReticleEffect_.cylinder_ = std::make_unique<PrimitiveDrawr>();
	hitReticleEffect_.cylinder_->TypeInit(PrimitiveType::Cylinder, 32);
	hitReticleEffect_.cylinder_->GetTransform().scale = {};
	hitReticleEffect_.cylinder_->SetColor({ 1.0f,1.0f,0.0f });
	hitReticleEffect_.cylinder_->SetBlendMode(BlendMode::kBlendModeAdd);
	hitReticleEffect_.cylinder_->GetRenderOptions().enabled = false;
	hitReticleEffect_.cylinder_->GetRenderOptions().offscreen = false;
	hitReticleEffect_.frame_ = 0.0f;
	hitReticleEffect_.axis_ = 0.0f;
}

void EnemyEffect::BulletPredictionInit()
{
	switch (enemy_->GetType())
	{
	case EnemyType::Ranged: { bulletPredictionEffect_.resize(1); }break;
	case EnemyType::RangedElite: { bulletPredictionEffect_.resize(3); }break;
	default:break;
	}

	// Planeの初期化
	for (auto& effect : bulletPredictionEffect_) {
		effect.plane_ = std::make_unique<PrimitiveDrawr>();
		effect.plane_->TypeInit(PrimitiveType::Plane);
		//effect.plane_->SetTexture("white1x1.png");
		effect.plane_->SetBlendMode(BlendMode::kBlendModeAdd);
		effect.plane_->SetColor(Vector3{ 1.0f,0.0f,0.0f });
		effect.plane_->SetAlpha(0.5f);
		effect.plane_->SetSceneRenderer();
		effect.plane_->GetRenderOptions().enabled = false;
	}
	if (!bulletPredictionEffect_.empty()) {
		// 弾を撃つ時のエフェクト
		bulletExplosionEmitter_ = std::make_unique<ParticleEmitter>("bulletExplosion");
		particleManager_->CreateParticleGroup(bulletExplosionEmitter_);
		bulletExplosionEmitter_->SetIsCreate(false);

		bulletSparkEmitter_ = std::make_unique<ParticleEmitter>("bulletSpark");
		particleManager_->CreateParticleGroup(bulletSparkEmitter_);
		bulletSparkEmitter_->SetIsCreate(false);

		bulletSmokeEmitter_ = std::make_unique<ParticleEmitter>("bulletSmoke");
		particleManager_->CreateParticleGroup(bulletSmokeEmitter_);
		bulletSmokeEmitter_->SetIsCreate(false);
	}

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
		if (enemy_->GetHitReticle()) { t = Easing::EaseInQuint(hitReticleEffect_.frame_); } else { t = Easing::EaseOutBack(hitReticleEffect_.frame_); }
		float scale = t;
		// スケールと回転を適応
		hitReticleEffect_.cylinder_->GetTransform().scale = { scale,scale,scale };
		hitReticleEffect_.axis_ += 0.1f;
		hitReticleEffect_.cylinder_->GetTransform().rotation =
			Quaternion::MakeRotateAxisAngleQuaternion(Vector3::ExprUnitY, hitReticleEffect_.axis_);
		Vector3 offset = { 0.0f,-0.5f,0.0f };
		hitReticleEffect_.cylinder_->GetTransform().translation = enemy_->GetTransform().translation_ + offset;
		hitReticleEffect_.cylinder_->Update();
	}
}

void EnemyEffect::BulletPredictionUpdate()
{
	switch (enemy_->GetType())
	{
	case EnemyType::Ranged:
	{
		RangedData data = enemy_->GetItem()->GetRangedData();
		auto& effect = bulletPredictionEffect_[0];

		// サイズを更新
		effect.plane_->GetTransform().scale = data.planeSize;
		// 座標を計算
		Quaternion quaternionY = Quaternion::ExtractYawQuaternion(enemy_->GetTransform().rotation_);
		Matrix4x4 rotateMatrix = Quaternion::MakeRotateMatrix(quaternionY);
		Vector3 offset = data.planeOffset.Transform(rotateMatrix);
		effect.plane_->GetTransform().translation = enemy_->GetTransform().translation_ + offset;
		// 回転を更新
		Quaternion rotateX = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::ExprUnitX, -std::numbers::pi_v<float> / 2.0f);
		Vector3 direction = (effect.plane_->GetTransform().translation - enemy_->GetTransform().translation_);
		direction.y = 0.0f;
		if (direction.Length() != 0.0f) {
			direction = direction.Normalize();
			Quaternion rotateY = Quaternion::DirectionToQuaternion(enemy_->GetTransform().rotation_, direction, 0.1f);
			effect.plane_->GetTransform().rotation = rotateY * rotateX;
		} else {
			effect.plane_->GetTransform().rotation = rotateX;
		}
		// 更新
		effect.plane_->GetUVTransform().position.x += 0.1f;
		effect.plane_->Update();
	}
		break;

	case EnemyType::RangedElite:
	{
		RangedEliteData data = enemy_->GetItem()->GetRangedEliteData();
		float rad = -data.bulletRadSpace;
		float pi = std::numbers::pi_v<float> / 4.0f;

		for (size_t i = 0; i < bulletPredictionEffect_.size(); ++i) {
			auto& effect = bulletPredictionEffect_[i];
			// サイズを更新
			effect.plane_->GetTransform().scale = data.planeSize[i];
			// 座標を計算
			Quaternion quaternionY = Quaternion::ExtractYawQuaternion(enemy_->GetTransform().rotation_);
			Matrix4x4 rotateMatrix = Quaternion::MakeRotateMatrix(quaternionY);
			Vector3 offset = data.planeOffset[i].Transform(rotateMatrix);
			effect.plane_->GetTransform().translation = enemy_->GetTransform().translation_ + offset;
			// 回転を更新
			Quaternion rotateX = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::ExprUnitX, std::numbers::pi_v<float> / 2.0f);
			Vector3 direction = (effect.plane_->GetTransform().translation - enemy_->GetTransform().translation_);
			direction.y = 0.0f;
			if (direction.Length() != 0.0f) {
				direction = direction.Normalize();
				quaternionY = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::ExprUnitY, pi * rad);
				Quaternion rotateY = Quaternion::DirectionToQuaternion(enemy_->GetTransform().rotation_, direction, 0.1f) * quaternionY;
				effect.plane_->GetTransform().rotation = rotateY * rotateX;
			} else {
				effect.plane_->GetTransform().rotation = rotateX;
			}
			// 更新
			effect.plane_->GetUVTransform().position.x += 0.1f;
			effect.plane_->Update();
			rad += data.bulletRadSpace;
		}
	}
		break;
	default:
		break;
	}
}
