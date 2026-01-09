#include "BossEffect.h"

#include <numbers>

#include "DeltaTimer.h"
#include "Vector3.h"

#include "Objects/Boss/Base/BossEnemy.h"

void BossEffect::Init()
{
	// 近接攻撃の初期化
	PrimitiveInit(meleeEffect_);
	meleeEffect_->SetTexture("circleWhite.png");
	PrimitiveInit(meleeTimeEffect_);
	meleeTimeEffect_->SetTexture("circleWhite.png");
	// ジャンプ攻撃の初期化
	PrimitiveInit(jumpAttackEffect_);
	jumpAttackEffect_->SetTexture("circleWhite.png");
	PrimitiveInit(jumpAttackTimeEffect_);
	jumpAttackTimeEffect_->SetTexture("circleWhite.png");
	// ダッシュ攻撃の初期化
	PrimitiveInit(dashAttackEffect_);
	dashAttackEffect_->SetTexture("gradationWhite.png");
	PrimitiveInit(dashAttackTimeEffect_);
	dashAttackTimeEffect_->SetTexture("gradationWhite.png");
	// ジャンプ時の煙エフェクト
	jumpDustEmitter_ = std::make_unique<ParticleEmitter>("jumpDust");
	particleManager_->CreateParticleGroup(jumpDustEmitter_);
	jumpDustEmitter_->SetIsCreate(false);
	// ダッシュ時のダウンエフェクト
	downLineEmitter_ = std::make_unique<ParticleEmitter>("downLine");
	particleManager_->CreateParticleGroup(downLineEmitter_);
	downLineEmitter_->SetIsCreate(false);
	downStarEmitter_ = std::make_unique<ParticleEmitter>("downStar");
	particleManager_->CreateParticleGroup(downStarEmitter_);
	downStarEmitter_->SetIsCreate(false);
	// ヒット時のエフェクト
	hitExplosionEmitter_ = std::make_unique<ParticleEmitter>("enemyHitExplosion");
	particleManager_->CreateParticleGroup(hitExplosionEmitter_);
	hitExplosionEmitter_->SetIsCreate(false);
	hitRingEmitter_ = std::make_unique<ParticleEmitter>("enemyHitRing");
	particleManager_->CreateParticleGroup(hitRingEmitter_);
	hitRingEmitter_->SetIsCreate(false);
}

void BossEffect::PrimitiveInit(std::unique_ptr<PrimitiveDrawr>& effect)
{
	effect = std::make_unique<PrimitiveDrawr>();
	effect->TypeInit(PrimitiveType::kPlane);
	effect->SetColor(Vector3::ExprUnitX);
	effect->SetAlpha(0.6f);
	effect->SetBlendMode(BlendMode::kBlendModeAdd);
	effect->GetRenderOptions().enabled = false;
	effect->GetTransform().scale = {};
}

void BossEffect::PrimitiveUpdate(std::unique_ptr<PrimitiveDrawr>& effect, const float time, const float size, const Vector3& offset)
{
	float scale = attackEffectTime_ / time;
	scale = std::clamp(scale, 0.0f, 1.0f);
	effect->GetTransform().scale = Vector3{ scale,scale,1.0f } * size;
	const float halfPi = std::numbers::pi_v<float> / 2.0f;
	Quaternion rotateX = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::ExprUnitX, halfPi);
	effect->GetTransform().rotation = boss_->GetTransform().rotation_ * rotateX;
	effect->GetTransform().translation = boss_->GetTransform().translation_ +
		offset.Transform(Quaternion::MakeRotateMatrix(boss_->GetTransform().rotation_));
	effect->GetTransform().translation.y = offset.y;
	effect->GetRenderOptions().enabled = true;
}

void BossEffect::PrimitiveUpdate(std::unique_ptr<PrimitiveDrawr>& effect, const float time, const Vector2& size, const Vector3& offset)
{
	float scale = attackEffectTime_ / time;
	scale = std::clamp(scale, 0.0f, 1.0f);
	effect->GetTransform().scale = Vector3{ scale * size.x,size.y,1.0f };
	const float halfPi = std::numbers::pi_v<float> / 2.0f;
	Quaternion rotateX = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::ExprUnitX, halfPi);
	effect->GetTransform().rotation = boss_->GetTransform().rotation_ * rotateX;
	effect->GetTransform().translation = boss_->GetTransform().translation_ +
		offset.Transform(Quaternion::MakeRotateMatrix(boss_->GetTransform().rotation_));
	effect->GetTransform().translation.y = offset.y;
	effect->GetRenderOptions().enabled = true;
}

void BossEffect::Update()
{
	meleeEffect_->Update();
	meleeTimeEffect_->Update();

	jumpAttackEffect_->Update();
	jumpAttackTimeEffect_->Update();
	
	dashAttackEffect_->Update();
	dashAttackTimeEffect_->Update();
}

void BossEffect::Draw()
{
	if (meleeEffect_->GetRenderOptions().enabled) {
		meleeEffect_->TypeDraw();
		meleeTimeEffect_->TypeDraw();
	}
	if (jumpAttackEffect_->GetRenderOptions().enabled) {
		jumpAttackEffect_->TypeDraw();
		jumpAttackTimeEffect_->TypeDraw();
	}
	if (dashAttackEffect_->GetRenderOptions().enabled) {
		dashAttackEffect_->TypeDraw();
		dashAttackTimeEffect_->TypeDraw();
	}
}

void BossEffect::SetAttackEffect(BossAttackEffect effect)
{
	// タイムを加算
	attackEffectTime_ += DeltaTimer::GetDeltaTime();

	switch (effect)
	{
	case BossAttackEffect::Melee:
	{
		// データを取得
		const auto& data = boss_->GetItems()->GetMeleeData();
		// エフェクトスケールを発生させる
		PrimitiveUpdate(meleeEffect_, data.attackEffectAppearTime,
			data.attackEffectSize, data.attackEffectOffset);
		PrimitiveUpdate(meleeTimeEffect_, data.attackStartupTime,
			data.attackEffectSize, data.attackEffectOffset);
	}
		break;
	case BossAttackEffect::JumpAttack:
	{
		// データを取得
		const auto& data = boss_->GetItems()->GetJumpAttackData();
		// エフェクトスケールを発生させる
		PrimitiveUpdate(jumpAttackEffect_, data.attackEffectAppearTime,
			data.attackEffectSize, data.attackEffectOffset);
		PrimitiveUpdate(jumpAttackTimeEffect_, data.airHoldTime + data.fallDownTime,
			data.attackEffectSize, data.attackEffectOffset);
	}
		break;
	case BossAttackEffect::DashAttack:
	{
		// データを取得
		const auto& data = boss_->GetItems()->GetDashAttackData();
		// エフェクトスケールを発生させる
		PrimitiveUpdate(dashAttackEffect_, data.attackEffectAppearTime,
			data.attackEffectSize, data.attackEffectOffset);
		PrimitiveUpdate(dashAttackTimeEffect_, data.attackStartupTime,
			data.attackEffectSize, data.attackEffectOffset);
	}
		break;
	default:
		break;
	}
}

void BossEffect::AttackEffectReset()
{
	meleeEffect_->GetRenderOptions().enabled = false;
	meleeTimeEffect_->GetRenderOptions().enabled = false;
	meleeEffect_->GetTransform().scale = {};
	meleeTimeEffect_->GetTransform().scale = {};

	jumpAttackEffect_->GetRenderOptions().enabled = false;
	jumpAttackEffect_->GetTransform().scale = {};
	jumpAttackTimeEffect_->GetRenderOptions().enabled = false;
	jumpAttackTimeEffect_->GetTransform().scale = {};

	dashAttackEffect_->GetRenderOptions().enabled = false;
	dashAttackEffect_->GetTransform().scale = {};
	dashAttackTimeEffect_->GetRenderOptions().enabled = false;
	dashAttackTimeEffect_->GetTransform().scale = {};
	attackEffectTime_ = 0.0f;
}

void BossEffect::OnceJumpEffect()
{
	jumpDustEmitter_->onceEmit();

	// パーティクルの座標を設定
	Quaternion rotate = boss_->GetTransform().rotation_;
	Vector3 position = boss_->GetTransform().translation_;

	jumpDustEmitter_->SetRotation(rotate);
	jumpDustEmitter_->SetPosition(position);
}

void BossEffect::OnceDownEffect()
{
	downLineEmitter_->onceEmit();

	// パーティクルの座標を設定
	Quaternion rotate = boss_->GetTransform().rotation_;
	Vector3 position = boss_->GetTransform().translation_;

	downLineEmitter_->SetRotation(rotate);
	downLineEmitter_->SetPosition(position);
}

void BossEffect::EmitDownStar(bool flag)
{
	downStarEmitter_->SetIsCreate(flag);

	// パーティクルの座標を設定
	Quaternion rotate = boss_->GetTransform().rotation_;
	Vector3 position = boss_->GetTransform().translation_;

	downStarEmitter_->SetRotation(rotate);
	downStarEmitter_->SetPosition(position);
}

void BossEffect::OnceHitExplosionEffect()
{
	hitExplosionEmitter_->onceEmit();
	hitRingEmitter_->onceEmit();

	// パーティクルの座標を設定
	Quaternion rotate = boss_->GetTransform().rotation_;
	Vector3 position = boss_->GetTransform().translation_;

	hitExplosionEmitter_->SetRotation(rotate);
	hitExplosionEmitter_->SetPosition(position);
	hitRingEmitter_->SetRotation(rotate);
	hitRingEmitter_->SetPosition(position);
}
