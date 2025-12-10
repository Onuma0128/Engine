#pragma once

#include <vector>
#include <memory>

#include "BaseUI.h"

#include "uis/select/SelectSystem.h"

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
	void SetKillCount(int num) { selectSystem_->SetKillCount(num); }

	/// <summary>
	/// 命中率を設定する
	/// </summary>
	/// <param name="num"></param>
	void SetHitRate(int num) { selectSystem_->SetHitRate(num); }

private:

	// 操作系のUI
	std::vector<std::unique_ptr<BaseUI>> controlUIs_;

	// セレクトUI
	std::unique_ptr<SelectSystem> selectSystem_ = nullptr;

	// フェード
	std::unique_ptr<BaseUI> sceneFade_ = nullptr;

};

