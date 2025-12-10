#pragma once

#include <memory>
#include <array>
#include <random>

#include "BaseUI.h"

#include "uis/player/PlayerCountUI.h"
#include "SelectUI.h"
#include "adjustItem/SelectAdjustItem.h"

/// <summary>
/// セレクトUIの表時や更新を管理するクラス
/// </summary>
class SelectSystem
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// セレクトUIのフェードイン
	/// </summary>
	void SelectUIFadeIn();

	/// <summary>
	/// セレクトUIが表示されているか判定
	/// </summary>
	/// <returns></returns>
	bool GetIsSelectIn()const { return updateSelectUI_; }

	/// <summary>
	/// キル数を設定する
	/// </summary>
	/// <param name="num"></param>
	void SetKillCount(int num) { killCount_ = num; }

	/// <summary>
	/// 命中率を設定する
	/// </summary>
	/// <param name="num"></param>
	void SetHitRate(int num) { hitRate_ = num; }

	/// <summary>
	/// シーンフェードを設定する
	/// </summary>
	/// <param name="sceneFade"></param>
	void SetSceneFade(BaseUI* sceneFade) { sceneFade_ = sceneFade; }

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

	// シーンフェード
	BaseUI* sceneFade_ = nullptr;
	bool isSceneFadeIn_ = false;

	// 位ごとに描画する
	CountUiOrder countUiOrder_ = CountUiOrder::First;
	float selectUiInterval_ = 0.0f;
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

