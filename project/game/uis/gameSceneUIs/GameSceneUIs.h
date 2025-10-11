#pragma once

#include <vector>
#include <memory>

#include "BaseUI.h"

#include "uis/select/SelectSystem.h"

class GameSceneUIs
{
public:

	void Init();

	void Update();

	void Draw();

private:

	// 操作系のUI
	std::vector<std::unique_ptr<BaseUI>> controlUIs_;

	// セレクトUI
	std::unique_ptr<SelectSystem> selectSystem_ = nullptr;

};

