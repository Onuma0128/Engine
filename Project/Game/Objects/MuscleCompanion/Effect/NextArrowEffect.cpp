#include "NextArrowEffect.h"

#include <numbers>

#include "DeltaTimer.h"

#include "Objects/MuscleCompanion/AdjustItem/CompanionAdjustItem.h"

void NextArrowEffect::Init()
{
	// 矢印エフェクトの初期化
	arrow_ = std::make_unique<PrimitiveDrawr>();
	arrow_->TypeInit(PrimitiveType::kPlane);
	arrow_->SetIsBillboard(true);
}

void NextArrowEffect::Update(const Vector3& position)
{
	// データを取得
	const auto& data = items_->GetEffectData();

	// タイム加算
	animaTime_ += DeltaTimer::GetDeltaTime();
	float t = animaTime_ / data.nextArrowAnimaTime;
	t = std::sinf(t * std::numbers::pi_v<float>);

	// スケールと座標を設定
	arrow_->GetTransform().scale = data.nextArrowScale + data.nextArrowVarianceScale * t;
	arrow_->GetTransform().translation = data.nextArrowPosition + data.nextArrowVariancePosition * t;
	arrow_->GetTransform().translation += position;

	// 更新
	arrow_->Update();
}

void NextArrowEffect::Draw()
{
	if (arrow_->GetRenderOptions().enabled) {
		arrow_->TypeDraw();
	}
}