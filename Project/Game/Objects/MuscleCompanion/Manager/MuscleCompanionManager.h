#pragma once

#include <memory>
#include <vector>

#include "Audio.h"
#include "Objects/MuscleCompanion/Base/MuscleCompanion.h"
#include "Objects/MuscleCompanion/AdjustItem/CompanionAdjustItem.h"

/// <summary>
/// プレイヤーの仲間を管理するクラス
/// </summary>
class MuscleCompanionManager
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();


	// セッター
	void SetPlayer(Player* player) { player_ = player; }
	void SetMapData(MapCollision* map) { mapData_ = map; }
	void SetCamera(GameCamera* camera) { camera_ = camera; }

private:

	/// <summary>
	/// 仲間の集合要求
	/// </summary>
	void GatherCompanions();

	/// <summary>
	/// 仲間同士の後続判定
	/// </summary>
	void FollowerDistanceCollision();

	/// <summary>
	/// 仲間が発射できるか判定
	/// </summary>
	/// <returns></returns>
	bool IsShotCompanion();

private:

	// プレイヤーのポインタ
	Player* player_ = nullptr;
	// マップデータのポインタ
	MapCollision* mapData_ = nullptr;
	// ゲームカメラ
	GameCamera* camera_ = nullptr;

	// 仲間の調整項目
	std::unique_ptr<CompanionAdjustItem> items_ = nullptr;

	// プレイヤーの仲間達
	std::vector<std::unique_ptr<MuscleCompanion>> companions_;

	// オーディオ
	std::unique_ptr<Audio> audio_ = nullptr;



};

