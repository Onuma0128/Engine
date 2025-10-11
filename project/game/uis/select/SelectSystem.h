#pragma once

#include <memory>
#include <array>

#include "uis/player/PlayerKillCountUI.h"
#include "SelectBackGroundUI.h"

class SelectSystem
{
public:

	void Init();

	void Update();

	void Draw();

private:

	// セレクトの背景画像
	std::array<std::unique_ptr<SelectBackGroundUI>, 2> backGroundUIs_;

	// 倒した数を表示する
	std::unique_ptr<PlayerKillCountUI> killCountUI_;

};

