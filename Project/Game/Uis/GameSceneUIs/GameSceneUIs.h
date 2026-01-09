#pragma once

#include <vector>
#include <memory>

#include "BaseUI.h"

#include "Uis/GameSceneUIs/PlayerControlUI.h"
#include "Uis/Select/SelectSystem.h"
#include "Uis/Player/NumberCountUI.h"
#include "Uis/GameSceneUIs/AdjustItem/GameUiAdjustItem.h"

// 前方宣言
class Player;
class EnemySpawnerFactory;

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
	void BossFadeUpdate();

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

	/// <summary>
	/// ゲーム上のUIを描画するかセットする
	/// </summary>
	/// <param name="flag"></param>
	void SetDrawGameUIs(bool flag) { isDrawGameUIs_ = flag; }

	/// <summary>
	/// アクセッサ
	/// </summary>
	/// <param name="player"></param>
	void SetPlayer(Player* player) { player_ = player; }
	void SetSpawner(EnemySpawnerFactory* spawner) { spawner_ = spawner; }

private:

	// プレイヤーのポインタ
	Player* player_ = nullptr;

	// 敵スポナーファクトリー
	EnemySpawnerFactory* spawner_ = nullptr;

	// 調整項目
	std::unique_ptr<GameUiAdjustItem> items_ = nullptr;

	// 操作系のUI
	std::unique_ptr<PlayerControlUI> controlUI_;

	// セレクトUI
	std::unique_ptr<SelectSystem> selectSystem_ = nullptr;

	// フェード
	std::unique_ptr<BaseUI> sceneFade_ = nullptr;
	std::unique_ptr<BaseUI> bossFade_ = nullptr;

	// 敵のキル数UI
	std::unique_ptr<NumberCountUI> killCountUI_ = nullptr;
	std::unique_ptr<NumberCountUI> maxKillCountUI_ = nullptr;
	std::unique_ptr<BaseUI> catUI_ = nullptr;
	uint32_t knockdownCount_ = 0;

	// ボス登場シーンのフェードステート
	enum class BossFadeState {
		StartIn,		// カメラ切り替え前のフェードイン
		StartOut,		// カメラ切り替え後のフェードアウト
		Active,			// ボス登場演出中
		RecoverIn,		// カメラ切り替え後のフェードイン
		RecoverOut,		// カメラ切り替え前のフェードアウト
		End,			// 終了
	};
	BossFadeState bossFadeState_ = BossFadeState::StartIn;
	void ChangeFadeState(BossFadeState changeState);

	// ボスフェードのタイム
	float bossFadeTime_ = -1.0f;
	// ゲーム内のUIを描画するか
	bool isDrawGameUIs_ = true;

};

