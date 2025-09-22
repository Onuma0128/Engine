#include "PlayerKillCountUI.h"

#include <algorithm> 

void PlayerKillCountUI::Init()
{
	numbers_.resize(3);

	for (uint32_t i = 0; i < numbers_.size(); ++i) {
		numbers_[i] = std::make_unique<NumbersUI>();
		Vector2 position = { static_cast<float>(600 + (i * 40)),50.0f };
		numbers_[i]->Init(position);
	}
}

void PlayerKillCountUI::Update(const uint32_t killCount)
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

void PlayerKillCountUI::Draw()
{
	for (auto& number : numbers_) {
		number->Draw();
	}
}


void NumbersUI::Init(const Vector2& position)
{
	Sprite::Initialize("numbers.png");
	Sprite::transform_.position = position;
	Sprite::anchorPoint_ = { 0.5f,0.5f };
	Sprite::transform_.size = { 33.0f,60.0f };
}

void NumbersUI::Update(const uint32_t number)
{
	const float digitWidth = 176.0f;
	const float digitHeight = 320.0f;

	Sprite::textureLeftTop_ = { digitWidth * number, 0.0f };
	Sprite::textureSize_ = { digitWidth, digitHeight };

	Sprite::Update();
}

void NumbersUI::Draw()
{
	Sprite::Draw();
}