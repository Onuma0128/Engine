#pragma once

#include <vector>
#include <memory>

#include "Sprite.h"

class NumbersUI : public Sprite
{
public:

	void Init(const Vector2& position);

	void Update(const uint32_t number);

	void Draw();

	void SetSize(const Vector2& size) { transform_.size = size; }
	void SetPosition(const Vector2& position) { transform_.position = position; }

private:
};

class PlayerCountUI
{
public:

	void Init();

	void Update(const uint32_t killCount);

	void Draw();

	void SetSize(const Vector2& size);
	void SetPosition(const Vector2& position);
	void SetInterval(const float interval);
	void SetAlpha(const float alpha);

private:

	// 数字のSprite
	std::vector<std::unique_ptr<NumbersUI>> numbers_;

	// 間隔
	float interval_;

};

