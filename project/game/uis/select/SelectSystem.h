#pragma once

#include <memory>
#include <array>
#include <random>

#include "BaseUI.h"

#include "uis/player/PlayerCountUI.h"
#include "SelectUI.h"
#include "adjustItem/SelectAdjustItem.h"

class SelectSystem
{
public:

	void Init();

	void Update();

	void Draw();

	void SelectUIFadeIn();

	// セレクトが出ているか判定
	bool GetIsSelectIn()const { return updateSelectUI_; }

	// キル数と命中率を受け取るセッター
	void SetKillCount(int num) { killCount_ = num; }
	void SetHitRate(int num) { hitRate_ = num; }

private:

	// インプット系を更新する
	void SelectInput();

	// キル数や命中率のUIを更新する
	void CounterUiUpdate();

	// キル数や命中率を表示する時の順番
	enum CountUiOrder{
		Interval	= 0,	// 始まるまでの間隔
		First		= 1,	// 一つ目
		Second		= 2,	// 二つ目
		Third		= 3,	// 三つ目
		End			= 4		// 終了
	};

private:

	// セレクトの背景画像
	// キルと命中率のUI
	// タイトルともう一度のUI
	std::array<std::unique_ptr<SelectUI>, 8> selectUIs_;

	// タイトルかもう一度かどちらを選んでいるかのIndex
	uint32_t targetIndex_ = 0u;

	// 倒した数を表示する
	std::unique_ptr<PlayerCountUI> killCountUI_ = nullptr;
	std::unique_ptr<PlayerCountUI> hitRateUI_ = nullptr;

	// 位ごとに描画する
	CountUiOrder countUiOrder_ = CountUiOrder::First;
	float clearCountUiTimer_ = 0.0f;

	// 描画するか決める
	bool isFadeIn_ = false;
	bool updateSelectUI_ = false;

	// セッターで受け取るキル数と命中率
	int killCount_ = 0;
	int hitRate_ = 0;

	// ランダムデバイス
	std::random_device seedGenerator_;

	// 調整項目
	std::unique_ptr<SelectAdjustItem> items_ = nullptr;

};

