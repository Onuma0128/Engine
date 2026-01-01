#pragma once

#include <vector>
#include <memory>

#include "BaseUI.h"

#include "Uis/Select/SelectSystem.h"
#include "Uis/Player/NumberCountUI.h"
#include "Uis/GameSceneUIs/AdjustItem/GameUiAdjustItem.h"

/// <summary>
/// ゲームシーンのUIを管理するクラス
/// </summary>
class GameSceneUIs
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
	/// セレクトが出ているか判定
	/// </summary>
	/// <returns></returns>
	bool GetIsSelectIn()const { return selectSystem_->GetIsSelectIn(); }
	
	/// <summary>
	/// キル数を設定する
	/// </summary>
	/// <param name="num"></param>
	void SetKillCount(int num) { 
		knockdownCount_ = num;
		selectSystem_->SetKillCount(num);
	}

private:

	// 調整項目
	std::unique_ptr<GameUiAdjustItem> items_ = nullptr;

	// 操作系のUI
	std::vector<std::unique_ptr<BaseUI>> controlUIs_;

	// セレクトUI
	std::unique_ptr<SelectSystem> selectSystem_ = nullptr;

	// フェード
	std::unique_ptr<BaseUI> sceneFade_ = nullptr;

	// 敵のキル数UI
	std::unique_ptr<NumberCountUI> killCountUI_ = nullptr;
	std::unique_ptr<NumberCountUI> maxKillCountUI_ = nullptr;
	std::unique_ptr<BaseUI> catUI_ = nullptr;
	uint32_t knockdownCount_ = 0;

};

