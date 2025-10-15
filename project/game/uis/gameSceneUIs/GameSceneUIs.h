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

	void SelectUIFadeIn();

	// セレクトが出ているか判定
	bool GetIsSelectIn()const { return selectSystem_->GetIsSelectIn(); }

	// キル数と命中率を受け取るセッター
	void SetKillCount(int num) { selectSystem_->SetKillCount(num); }
	void SetHitRate(int num) { selectSystem_->SetHitRate(num); }

private:

	// 操作系のUI
	std::vector<std::unique_ptr<BaseUI>> controlUIs_;

	// セレクトUI
	std::unique_ptr<SelectSystem> selectSystem_ = nullptr;

};

