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

    // 遘ｻ蜍墓凾縺ｮ繧ｨ繝輔ぉ繧ｯ繝・
	moveDustEmitter_ = std::make_shared<NumaEngine::ParticleEmitter>("moveDust");
	particleManager_->CreateParticleGroup(moveDustEmitter_);
	moveDustEmitter_->SetIsCreate(false);

	// 繝偵ャ繝域凾縺ｮ繧ｨ繝輔ぉ繧ｯ繝・
	hitEmitter_ = std::make_shared<NumaEngine::ParticleEmitter>("hitDamage");
	particleManager_->CreateParticleGroup(hitEmitter_);
	hitEmitter_->SetIsCreate(false);

	hitExplosionEmitter_ = std::make_shared<NumaEngine::ParticleEmitter>("enemyHitExplosion");
	particleManager_->CreateParticleGroup(hitExplosionEmitter_);
	hitExplosionEmitter_->SetIsCreate(false);
	hitExplosionBlueEmitter_ = std::make_shared<NumaEngine::ParticleEmitter>("enemyHitExplosionBlue");
	particleManager_->CreateParticleGroup(hitExplosionBlueEmitter_);
	hitExplosionBlueEmitter_->SetIsCreate(false);

	hitRingEmitter_ = std::make_shared<NumaEngine::ParticleEmitter>("enemyHitRing");
	particleManager_->CreateParticleGroup(hitRingEmitter_);
	hitRingEmitter_->SetIsCreate(false);
	hitRingBlueEmitter_ = std::make_shared<NumaEngine::ParticleEmitter>("enemyHitRingBlue");
	particleManager_->CreateParticleGroup(hitRingBlueEmitter_);
	hitRingBlueEmitter_->SetIsCreate(false);

	// 豁ｻ莠｡譎ゅ・繧ｨ繝輔ぉ繧ｯ繝・
	deadEmitter_ = std::make_shared<NumaEngine::ParticleEmitter>("enemyDead");
	particleManager_->CreateParticleGroup(deadEmitter_);
	deadEmitter_->SetIsCreate(false);

	enemyMeleeAttack_ = std::make_shared<NumaEngine::ParticleEmitter>("enemyMeleeAttack");
	particleManager_->CreateParticleGroup(enemyMeleeAttack_);
	enemyMeleeAttack_->SetIsCreate(false);
}

void EnemyEffect::Update()
{
	HitReticleUpdate();

	BulletPredictionUpdate();

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

	// 繝代・繝・ぅ繧ｯ繝ｫ縺ｮ蠎ｧ讓吶ｒ險ｭ螳・
	NumaEngine::Quaternion rotate = transform.rotation_;
	NumaEngine::Vector3 position = transform.translation_;

	moveDustEmitter_->SetRotation(rotate);
	moveDustEmitter_->SetPosition(position);
}

void EnemyEffect::OnceBulletEffect(const WorldTransform& transform)
{
	// Particle繧剃ｸ蝗樒函謌・
	bulletExplosionEmitter_->onceEmit();
	bulletSparkEmitter_->onceEmit();
	bulletSmokeEmitter_->onceEmit();

	// 繝代・繝・ぅ繧ｯ繝ｫ縺ｮ蠎ｧ讓吶ｒ險ｭ螳・
	NumaEngine::Quaternion rotate = transform.rotation_;
	NumaEngine::Vector3 position = transform.translation_;

	// 辷・匱
	bulletExplosionEmitter_->SetPosition(position);
	bulletExplosionEmitter_->SetRotation(rotate);
	// 轣ｫ闃ｱ
	bulletSparkEmitter_->SetPosition(position);
	bulletSparkEmitter_->SetRotation(rotate);
	// 辣・
	bulletSmokeEmitter_->SetPosition(position);
	bulletSmokeEmitter_->SetRotation(rotate);
}

void EnemyEffect::OnceBulletHitEffect(const WorldTransform& transform)
{
	hitEmitter_->onceEmit();

	// 繝代・繝・ぅ繧ｯ繝ｫ縺ｮ蠎ｧ讓吶ｒ險ｭ螳・
	NumaEngine::Quaternion rotate = transform.rotation_;
	NumaEngine::Vector3 position = transform.translation_;

	hitEmitter_->SetRotation(rotate);
	hitEmitter_->SetPosition(position);
}

void EnemyEffect::OnceBulletHitExplosionEffect(const WorldTransform& transform)
{
	hitExplosionEmitter_->onceEmit();
	hitRingEmitter_->onceEmit();

	// 繝代・繝・ぅ繧ｯ繝ｫ縺ｮ蠎ｧ讓吶ｒ險ｭ螳・
	NumaEngine::Quaternion rotate = transform.rotation_;
	NumaEngine::Vector3 position = transform.translation_;

	hitExplosionEmitter_->SetRotation(rotate);
	hitExplosionEmitter_->SetPosition(position);
	hitRingEmitter_->SetRotation(rotate);
	hitRingEmitter_->SetPosition(position);
}

void EnemyEffect::OnceBulletHitExplosionBlueEffect(const WorldTransform& transform)
{
	hitExplosionBlueEmitter_->onceEmit();
	hitRingBlueEmitter_->onceEmit();

	// 繝代・繝・ぅ繧ｯ繝ｫ縺ｮ蠎ｧ讓吶ｒ險ｭ螳・
	NumaEngine::Quaternion rotate = transform.rotation_;
	NumaEngine::Vector3 position = transform.translation_;

	hitExplosionBlueEmitter_->SetRotation(rotate);
	hitExplosionBlueEmitter_->SetPosition(position);
	hitRingBlueEmitter_->SetRotation(rotate);
	hitRingBlueEmitter_->SetPosition(position);
}

void EnemyEffect::SetMeleeAttackEffect(const WorldTransform& transform)
{
	// 繝代・繝・ぅ繧ｯ繝ｫ縺ｮ蠎ｧ讓吶ｒ險ｭ螳・
	NumaEngine::Quaternion rotate = transform.rotation_;
	NumaEngine::Vector3 position = transform.translation_;

	enemyMeleeAttack_->SetRotation(rotate);
	enemyMeleeAttack_->SetPosition(position);
}

void EnemyEffect::SetBulletPredictionEffect(bool flag)
{
	for (auto& effect : bulletPredictionEffect_) {
		effect->SetEnabledDraw(flag);
	}
}

void EnemyEffect::HitReticleInit()
{
	hitReticleEffect_.cylinder_ = std::make_unique<PrimitiveDrawr>();
	hitReticleEffect_.cylinder_->TypeInit(PrimitiveType::kCylinder, 32);
	hitReticleEffect_.cylinder_->GetTransform().scale = {};
	hitReticleEffect_.cylinder_->SetColor(NumaEngine::Vector3::ExprUnitX + NumaEngine::Vector3::ExprUnitY);
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
	case EnemyType::kRanged: { bulletPredictionEffect_.resize(1); }break;
	case EnemyType::kRangedElite: { bulletPredictionEffect_.resize(3); }break;
	default:break;
	}

	// Plane縺ｮ蛻晄悄蛹・
	for (auto& effect : bulletPredictionEffect_) {
		effect = std::make_unique<EnemyBulletPredictionEffect>();
		effect->Init();
	}
	if (!bulletPredictionEffect_.empty()) {
        // 蠑ｾ繧呈茶縺､譎ゅ・繧ｨ繝輔ぉ繧ｯ繝・
		bulletExplosionEmitter_ = std::make_shared<NumaEngine::ParticleEmitter>("bulletExplosion");
		particleManager_->CreateParticleGroup(bulletExplosionEmitter_);
		bulletExplosionEmitter_->SetIsCreate(false);

		bulletSparkEmitter_ = std::make_shared<NumaEngine::ParticleEmitter>("bulletSpark");
		particleManager_->CreateParticleGroup(bulletSparkEmitter_);
		bulletSparkEmitter_->SetIsCreate(false);

		bulletSmokeEmitter_ = std::make_shared<NumaEngine::ParticleEmitter>("bulletSmoke");
		particleManager_->CreateParticleGroup(bulletSmokeEmitter_);
		bulletSmokeEmitter_->SetIsCreate(false);
	}

}

void EnemyEffect::HitReticleUpdate()
{
	float deltaTime = 1.0f / 60.0f;

	// 繝偵ャ繝医＠縺ｦ縺・ｋ縺ｪ繧画緒逕ｻ繧偵☆繧・
	if (enemy_->GetHitReticle()) {
		hitReticleEffect_.cylinder_->GetRenderOptions().enabled = true;
		hitReticleEffect_.frame_ += deltaTime;

		// 繝偵ャ繝医′邨ゅｏ縺｣縺溘ｉ
	} else {
		hitReticleEffect_.frame_ -= deltaTime * 2.0f;

		// 謠冗判繧貞・繧・
		if (hitReticleEffect_.frame_ <= 0.0f) {
			hitReticleEffect_.cylinder_->GetRenderOptions().enabled = false;
		}
	}

	// Cylinder縺梧緒逕ｻ縺輔ｌ縺ｦ縺・ｋ縺ｪ繧・
	if (hitReticleEffect_.cylinder_->GetRenderOptions().enabled) {
		// frame繧偵け繝ｩ繝ｳ繝励＠縺ｦ繧､繝ｼ繧ｸ繝ｳ繧ｰ繧呈寺縺代ｋ
		hitReticleEffect_.frame_ = std::clamp(hitReticleEffect_.frame_, 0.0f, 1.0f);
		float t = 0.0f;
		if (enemy_->GetHitReticle()) { t = NumaEngine::Easing::EaseInQuint(hitReticleEffect_.frame_); } else { t = NumaEngine::Easing::EaseOutBack(hitReticleEffect_.frame_); }
		float scale = t;
		// 繧ｹ繧ｱ繝ｼ繝ｫ縺ｨ蝗櫁ｻ｢繧帝←蠢・
		hitReticleEffect_.cylinder_->GetTransform().scale = { scale,scale,scale };
		hitReticleEffect_.axis_ += 0.1f;
		hitReticleEffect_.cylinder_->GetTransform().rotation =
			NumaEngine::Quaternion::MakeRotateAxisAngleQuaternion(NumaEngine::Vector3::ExprUnitY, hitReticleEffect_.axis_);
		NumaEngine::Vector3 offset = { 0.0f,-0.5f,0.0f };
		hitReticleEffect_.cylinder_->GetTransform().translation = enemy_->GetTransform().translation_ + offset;
		hitReticleEffect_.cylinder_->Update();
	}
}

void EnemyEffect::BulletPredictionUpdate()
{
	switch (enemy_->GetType())
	{
	case EnemyType::kRanged:
	{
		const auto& data = enemy_->GetItem()->GetRangedData();
		auto& effect = bulletPredictionEffect_[0];

		// 繧ｵ繧､繧ｺ繧呈峩譁ｰ
		effect->SetScale(data.planeSize);
		// 蠎ｧ讓吶ｒ險育ｮ・
		NumaEngine::Quaternion quaternionY = NumaEngine::Quaternion::ExtractYawQuaternion(enemy_->GetTransform().rotation_);
     NumaEngine::Matrix4x4 rotateMatrix = NumaEngine::Quaternion::MakeRotateMatrix(quaternionY);
		NumaEngine::Vector3 offset = data.planeOffset.Transform(rotateMatrix) + enemy_->GetTransform().translation_;
		effect->SetTranslate(offset);
		// 蝗櫁ｻ｢繧呈峩譁ｰ
		NumaEngine::Quaternion rotateX = NumaEngine::Quaternion::MakeRotateAxisAngleQuaternion(NumaEngine::Vector3::ExprUnitX, -std::numbers::pi_v<float> / 2.0f);
		NumaEngine::Vector3 direction = (offset - enemy_->GetTransform().translation_);
		direction.y = 0.0f;
		if (direction.Length() != 0.0f) {
			direction = direction.Normalize();
			NumaEngine::Quaternion rotateY = NumaEngine::Quaternion::DirectionToQuaternion(enemy_->GetTransform().rotation_, direction, 0.1f);
			effect->SetRotate(rotateY * rotateX);
		} else {
			effect->SetRotate(rotateX);
		}
		// 譖ｴ譁ｰ
		effect->SetAddUvPosition(NumaEngine::Vector2{ 0.1f,0.0f });
		effect->SetEnemyPosition(enemy_->GetTransform().translation_);
		effect->Update();
	}
		break;

	case EnemyType::kRangedElite:
	{
		const auto& data = enemy_->GetItem()->GetRangedEliteData();
		float rad = -data.bulletRadSpace;
		float pi = std::numbers::pi_v<float> / 4.0f;

		for (size_t i = 0; i < bulletPredictionEffect_.size(); ++i) {
			auto& effect = bulletPredictionEffect_[i];
			// 繧ｵ繧､繧ｺ繧呈峩譁ｰ
			effect->SetScale(data.planeSize[i]);
			// 蠎ｧ讓吶ｒ險育ｮ・
			NumaEngine::Quaternion quaternionY = NumaEngine::Quaternion::ExtractYawQuaternion(enemy_->GetTransform().rotation_);
         NumaEngine::Matrix4x4 rotateMatrix = NumaEngine::Quaternion::MakeRotateMatrix(quaternionY);
			NumaEngine::Vector3 offset = data.planeOffset[i].Transform(rotateMatrix) + enemy_->GetTransform().translation_;
			effect->SetTranslate(offset);
			// 蝗櫁ｻ｢繧呈峩譁ｰ
			NumaEngine::Quaternion rotateX = NumaEngine::Quaternion::MakeRotateAxisAngleQuaternion(NumaEngine::Vector3::ExprUnitX, std::numbers::pi_v<float> / 2.0f);
			NumaEngine::Vector3 direction = (offset - enemy_->GetTransform().translation_);
			direction.y = 0.0f;
			if (direction.Length() != 0.0f) {
				direction = direction.Normalize();
				quaternionY = NumaEngine::Quaternion::MakeRotateAxisAngleQuaternion(NumaEngine::Vector3::ExprUnitY, pi * rad);
				NumaEngine::Quaternion rotateY = NumaEngine::Quaternion::DirectionToQuaternion(enemy_->GetTransform().rotation_, direction, 0.1f) * quaternionY;
				effect->SetRotate(rotateY * rotateX);
			} else {
				effect->SetRotate(rotateX);
			}
			// 譖ｴ譁ｰ
			effect->SetAddUvPosition(NumaEngine::Vector2{ 0.1f,0.0f });
			effect->SetEnemyPosition(enemy_->GetTransform().translation_);
			effect->Update();
			rad += data.bulletRadSpace;
		}
	}
		break;
	default:
		break;
	}
}


