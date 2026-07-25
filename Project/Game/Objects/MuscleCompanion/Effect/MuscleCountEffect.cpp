#include "MuscleCountEffect.h"

#include <numbers>

#include "DeltaTimer.h"

void MuscleCountEffect::Init()
{
	// 仲間の数を表示するエフェクトの初期化
	maxCountEffect_ = std::make_unique<PrimitiveDrawr>();
	maxCountEffect_->TypeInit(PrimitiveType::kPlane);
	maxCountEffect_->SetTexture("numbers.png");
	maxCountEffect_->SetIsBillboard(true);
	maxCountEffect_->GetRenderOptions().enabled = true;
	nowCountEffect_ = std::make_unique<PrimitiveDrawr>();
	nowCountEffect_->TypeInit(PrimitiveType::kPlane);
	nowCountEffect_->SetTexture("numbers.png");
	nowCountEffect_->SetIsBillboard(true);
	nowCountEffect_->GetRenderOptions().enabled = true;
	catEffect_ = std::make_unique<PrimitiveDrawr>();
	catEffect_->TypeInit(PrimitiveType::kPlane);
	catEffect_->SetTexture("catUI.png");
	catEffect_->SetIsBillboard(true);
	catEffect_->GetRenderOptions().enabled = true;
}

void MuscleCountEffect::Update(const NumaEngine::Vector3& position)
{
	// タイム加算
	timer_ -= DeltaTimer::GetDeltaTime();

	// スケールと座標を設定
	float t = timer_;
	maxCountEffect_->SetAlpha(t);
	nowCountEffect_->SetAlpha(t);
	catEffect_->SetAlpha(t);

	// サイズ設定
	NumaEngine::Vector3 scale = NumaEngine::Vector3::ExprUnitXYZ * 0.33f;
	nowCountEffect_->GetTransform().scale = scale;
	maxCountEffect_->GetTransform().scale = scale;
	catEffect_->GetTransform().scale = scale;
	// 番号を設定
	Transform2D uv{};
	uv.size = { 1.0f / 10.0f, 1.0f };
	uv.rotate = 0.0f;
	if (nowCount_ == 0) { nowCount_ = 1; }
	uv.position = { static_cast<float>(nowCount_ - 1), 0.0f };
	nowCountEffect_->SetUVTransform(uv);
	uv.position = { static_cast<float>(maxCount_), 0.0f };
	maxCountEffect_->SetUVTransform(uv);
	// 座標設定
	catEffect_->GetTransform().translation =
		position + NumaEngine::Vector3::ExprUnitY * 2.0f;
	nowCountEffect_->GetTransform().translation =
		position + NumaEngine::Vector3::ExprUnitY * 2.0f - NumaEngine::Vector3::ExprUnitX * 0.4f;
	maxCountEffect_->GetTransform().translation =
		position + NumaEngine::Vector3::ExprUnitY * 2.0f + NumaEngine::Vector3::ExprUnitX * 0.4f;

	// 更新
	maxCountEffect_->Update();
	nowCountEffect_->Update();
	catEffect_->Update();
}

void MuscleCountEffect::Draw()
{
	if (maxCountEffect_->GetRenderOptions().enabled) {
		maxCountEffect_->TypeDraw();
	}
	if (nowCountEffect_->GetRenderOptions().enabled) {
		nowCountEffect_->TypeDraw();
	}
	if (catEffect_->GetRenderOptions().enabled) {
		catEffect_->TypeDraw();
	}
}
