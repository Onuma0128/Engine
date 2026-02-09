#pragma once

#include <memory>
#include <array>
#include <random>

#include "BaseUI.h"
#include "Audio.h"

#include "Uis/Player/NumberCountUI.h"
#include "SelectUI.h"
#include "AdjustItem/SelectAdjustItem.h"

// 前方宣言
class GameSceneUIs;
class Player;
class EnemySpawnerFactory;
class BossEnemy;
class GameCamera;
class MuscleCompanionManager;

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
	void SelectUIFadeIn(bool isClear);

	/// <summary>
	/// セレクトUIが表示されているか判定
	/// </summary>
	/// <returns></returns>
	bool GetIsSelectIn()const { return updateSelectUI_; }

	/// <summary>
	/// シーンフェードを設定する
	/// </summary>
	/// <param name="sceneFade"></param>
	void SetGameSceneUis(GameSceneUIs* gameSceneUis) { gameSceneUis_ = gameSceneUis; }

	/// <summary>
	/// セレクトをリセットする
	/// </summary>
	void Reset();

	void BossStart();

	/// <summary>
	/// アクセッサ
	/// </summary>
	/// <param name="player"></param>
	void SetPlayer(Player* player) { player_ = player; }
	void SetSpawner(EnemySpawnerFactory* spawner) { spawner_ = spawner; }
	void SetBossEnemy(BossEnemy* boss) { boss_ = boss; }
	void SetCamera(GameCamera* camera) { camera_ = camera; }
	void SetCompanionManager(MuscleCompanionManager* manager) { companionManager_ = manager; }
	void SetAudio(Audio* audio) { audio_ = audio; }

private:

	// インプット系を更新する
	void SelectInput();

private:

	// プレイヤーのポインタ
	Player* player_ = nullptr;
	// 敵スポナーファクトリー
	EnemySpawnerFactory* spawner_ = nullptr;
	// ボス敵のポインタ
	BossEnemy* boss_ = nullptr;
	// 仲間の管理ポインタ
	MuscleCompanionManager* companionManager_ = nullptr;
	// ゲームカメラのポインタ
	GameCamera* camera_ = nullptr;
	// BGM
	Audio* audio_ = nullptr;
	// ゲームシーンのUIポインタ
	GameSceneUIs* gameSceneUis_ = nullptr;

	// 調整項目
	std::unique_ptr<SelectAdjustItem> items_ = nullptr;
	// セレクトの背景画像
	// キルと命中率のUI
	// タイトルともう一度のUI
	static const size_t kSelectUiSize_ = 7;
	std::array<std::unique_ptr<SelectUI>, kSelectUiSize_> selectUIs_;

	// タイトルかもう一度かどちらを選んでいるかのIndex
	uint32_t targetIndex_ = 0u;
	float targetTime_ = 0.0f;
	// セレクトに移行したか
	bool isSceneFadeIn_ = false;
	// 描画するか決める
	bool isFadeIn_ = false;
	bool updateSelectUI_ = false;
	float clearCountUiTimer_ = 0.0f;

	// ランダムデバイス
	std::random_device seedGenerator_;

};

