#pragma once

#include <memory>
#include <vector>

#include "Audio.h"
#include "Objects/MuscleCompanion/Base/MuscleCompanion.h"
#include "Objects/MuscleCompanion/Effect/CompanionEffect.h"
#include "Objects/MuscleCompanion/Effect/NextArrowEffect.h"
#include "Objects/MuscleCompanion/AdjustItem/CompanionAdjustItem.h"
#include "Objects/MuscleCompanion/PredictionObject/PredictionObjects.h"

// 前方宣言
class BossEnemy;

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
	void SetBossEnemy(BossEnemy* boss) { boss_ = boss; }

	/// <summary>
	/// 仲間同士の中心座標を返す
	/// </summary>
	/// <returns></returns>
	const Vector3 CompanionCenterPosition();

	/// <summary>
	/// 仲間のデータをリセットする
	/// </summary>
	void Reset();

private:

	/// <summary>
	/// 仲間の集合要求
	/// </summary>
	void GatherCompanions();

	/// <summary>
	/// クリア時の強制集合
	/// </summary>
	void ClearGatherCompanions();

	/// <summary>
	/// 仲間同士の後続判定
	/// </summary>
	void FollowerDistanceCollision();

	/// <summary>
	/// 仲間が発射できるか判定
	/// </summary>
	/// <returns></returns>
	bool IsShotCompanion();

	/// <summary>
	/// エフェクトを更新する
	/// </summary>
	void UpdateEffect();

private:

	// プレイヤーのポインタ
	Player* player_ = nullptr;
	// ボスのポインタ
	BossEnemy* boss_ = nullptr;
	// マップデータのポインタ
	MapCollision* mapData_ = nullptr;
	// ゲームカメラ
	GameCamera* camera_ = nullptr;

	// 仲間の調整項目
	std::unique_ptr<CompanionAdjustItem> items_ = nullptr;
	// プレイヤーの仲間達
	std::vector<std::unique_ptr<MuscleCompanion>> companions_;
	// 発射する時の予測線
	std::unique_ptr<PredictionObjects> predictionObjects_ = nullptr;
	// 次発射の仲間エフェクト
	std::unique_ptr<NextArrowEffect> arrowEffect_ = nullptr;
	// オーディオ
	std::unique_ptr<Audio> audio_ = nullptr;

	// 仲間がクリアステートに入る
	bool isClear_ = false;
	float clearStateTime_ = 0.0f;

};

