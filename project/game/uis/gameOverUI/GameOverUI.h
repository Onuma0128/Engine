#pragma once

#include "Sprite.h"

#include "BaseUI.h"

class GameOverUI : public BaseUI
{
public:

	void Init();

	void Update()override;

	void Draw()override;
};

