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

private:
};

class PlayerKillCountUI
{
public:

	void Init();

	void Update(const uint32_t killCount);

	void Draw();

private:

	// 数字のSprite
	std::vector<std::unique_ptr<NumbersUI>> numbers_;

};

