#include "NumberCountUI.h"

#include <algorithm> 

#include "DeltaTimer.h"

void NumberCountUI::Init(bool isNoiseTexture)
{
	const size_t kNumNumber = 3;
	numbers_.resize(kNumNumber);

	interval_ = 56.0f;

	for (uint32_t i = 0; i < numbers_.size(); ++i) {
		numbers_[i] = std::make_unique<NumbersUI>();
		Vector2 position = { static_cast<float>(600 + (i * interval_)),50.0f };
		numbers_[i]->Init(position, isNoiseTexture);
	}
}

void NumberCountUI::Update(const uint32_t killCount)
{
	uint32_t clamped = std::clamp(killCount, 0u, 999u);
	prevNumber_ = clamped;

	// 100・10・1 の各位を取り出す
	uint32_t digits[3] = {
		(clamped / 100) % 10,
		(clamped / 10) % 10,
		clamped % 10
	};

	// NumbersUIを更新
	for (size_t i = 0; i < numbers_.size(); ++i) {
		numbers_[i]->Update(digits[i]);
	}
	// 二桁目と三桁目が0なら描画しない
	bool showHundreds = (digits[0] != 0);
	bool showTens = (digits[0] != 0) || (digits[1] != 0);
	bool showOnes = true;

	numbers_[0]->GetRenderOptions().enabled = showHundreds;
	numbers_[1]->GetRenderOptions().enabled = showTens;
	numbers_[2]->GetRenderOptions().enabled = showOnes;
}

void NumberCountUI::MochiPuniScale(const uint32_t killCount, const float mochiPuniScale)
{
	if(killCount != prevNumber_) {
		mochiPuniTime_ = 0.0f;
	}

	mochiPuniTime_ += DeltaTimer::GetDeltaTime() * 3.0f;
	mochiPuniTime_ = std::clamp(mochiPuniTime_, 0.0f, 1.0f);
	Vector2 scale = Vector2::MochiPuniScaleNormalized(mochiPuniTime_);
	for (size_t i = 0; i < numbers_.size(); ++i) {
		numbers_[i]->GetTransform().size = scale * mochiPuniScale;
	}
}

void NumberCountUI::Draw()
{
	for (auto& number : numbers_) {
		if (number->GetRenderOptions().enabled) {
			number->Draw();
		}
	}
}

void NumberCountUI::SetSize(const Vector2& size)
{
	for (auto& number : numbers_) {
		number->SetSize(size);
	}
}

void NumberCountUI::SetPosition(const Vector2& position)
{
	int count = -1;
	for (auto& number : numbers_) {
		float posX = static_cast<float>(count) * interval_;
		number->SetPosition(position + Vector2{ posX ,0.0f });
		++count;
	}
}

void NumberCountUI::SetInterval(const float interval)
{
	interval_ = interval;
}

void NumberCountUI::SetAlpha(const float alpha)
{
	for (auto& number : numbers_) {
		number->SetColor(Vector4{ 1.0f,1.0f,1.0f,alpha });
	}
}

void NumberCountUI::SetDissolvePrames(const float threshold, const float edgeWidth, const Vector3& edgeColor)
{
	for (auto& number : numbers_) {
		number->SetDissolveThreshold(threshold);
		number->SetDissolveEdgeWidth(edgeWidth);
		number->SetDissolveEdgeColor(edgeColor);
	}
}


void NumbersUI::Init(const Vector2& position, bool isNoiseTexture)
{
	Sprite::Initialize("numbers.png", isNoiseTexture);
	if(isNoiseTexture) {
		Sprite::SetNoiseTexture("gradationTexture.png");
	}
	Sprite::transform_.position = position;
	Sprite::anchorPoint_ = { 0.5f,0.5f };
	Sprite::transform_.size = { 80.0f,80.0f };
}

void NumbersUI::Update(const uint32_t number)
{
	const float digitWidth = 128.0f;
	const float digitHeight = 128.0f;

	Sprite::textureLeftTop_ = { digitWidth * number, 0.0f };
	Sprite::textureSize_ = { digitWidth, digitHeight };

	Sprite::Update();
}

void NumbersUI::Draw()
{
	Sprite::Draw();
}