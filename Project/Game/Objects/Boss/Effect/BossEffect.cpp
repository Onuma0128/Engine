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
	// ジャンプ攻撃の初期化
	PrimitiveInit(jumpAttackEffect_);
	jumpAttackEffect_->SetTexture("circleWhite.png");
	// ダッシュ攻撃の初期化
	PrimitiveInit(dashAttackEffect_);
	dashAttackEffect_->SetTexture("white1x1.png");
}

void BossEffect::PrimitiveInit(std::unique_ptr<PrimitiveDrawr>& effect)
{
	effect = std::make_unique<PrimitiveDrawr>();
	effect->TypeInit(PrimitiveType::kPlane);
	effect->SetColor(Vector3::ExprUnitX);
	effect->SetAlpha(0.5f);
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
	jumpAttackEffect_->Update();
	dashAttackEffect_->Update();
}

void BossEffect::Draw()
{
	if (meleeEffect_->GetRenderOptions().enabled) {
		meleeEffect_->TypeDraw();
	}
	if (jumpAttackEffect_->GetRenderOptions().enabled) {
		jumpAttackEffect_->TypeDraw();
	}
	if (dashAttackEffect_->GetRenderOptions().enabled) {
		dashAttackEffect_->TypeDraw();
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
	}
		break;
	case BossAttackEffect::JumpAttack:
	{
		// データを取得
		const auto& data = boss_->GetItems()->GetJumpAttackData();
		// エフェクトスケールを発生させる
		PrimitiveUpdate(jumpAttackEffect_, data.attackEffectAppearTime,
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
	}
		break;
	default:
		break;
	}
}

void BossEffect::AttackEffectReset()
{
	meleeEffect_->GetRenderOptions().enabled = false;
	jumpAttackEffect_->GetRenderOptions().enabled = false;
	dashAttackEffect_->GetRenderOptions().enabled = false;
	meleeEffect_->GetTransform().scale = {};
	jumpAttackEffect_->GetTransform().scale = {};
	dashAttackEffect_->GetTransform().scale = {};
	attackEffectTime_ = 0.0f;
}
