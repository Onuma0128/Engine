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

	void SetPosition(const Vector2& position) { transform_.position = position; }

private:
};

class PlayerKillCountUI
{
public:

	void Init();

	void Update(const uint32_t killCount);

	void Draw();

	void SetPosition(const Vector2& position);

private:

	// 数字のSprite
	std::vector<std::unique_ptr<NumbersUI>> numbers_;

};

