#include "PlayerCountUI.h"

#include <algorithm> 

void PlayerCountUI::Init()
{
	numbers_.resize(3);

	interval_ = 52.0f;

	for (uint32_t i = 0; i < numbers_.size(); ++i) {
		numbers_[i] = std::make_unique<NumbersUI>();
		Vector2 position = { static_cast<float>(600 + (i * interval_)),50.0f };
		numbers_[i]->Init(position);
	}
}

void PlayerCountUI::Update(const uint32_t killCount)
{
	uint32_t clamped = std::clamp(killCount, 0u, 999u);

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
}

void PlayerCountUI::Draw()
{
	for (auto& number : numbers_) {
		number->Draw();
	}
}

void PlayerCountUI::SetSize(const Vector2& size)
{
	for (auto& number : numbers_) {
		number->SetSize(size);
	}
}

void PlayerCountUI::SetPosition(const Vector2& position)
{
	int count = -1;
	for (auto& number : numbers_) {
		float posX = static_cast<float>(count) * interval_;
		number->SetPosition(position + Vector2{ posX ,0.0f });
		++count;
	}
}

void PlayerCountUI::SetInterval(const float interval)
{
	interval_ = interval;
}

void PlayerCountUI::SetAlpha(const float alpha)
{
	for (auto& number : numbers_) {
		number->SetColor(Vector4{ 1.0f,1.0f,1.0f,alpha });
	}
}


void NumbersUI::Init(const Vector2& position)
{
	Sprite::Initialize("numbers.png");
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