#pragma once

#include <list>

#include "Objects/Boss/AdjustItem/BossAdjustItem.h"
#include "Objects/Boss/State/BossBaseState.h"

// 前方宣言
class BossEnemy;

/// <summary>
/// ボスのステートをスコア付けして評価するクラス
/// </summary>
class BossStateEvaluator
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

	// セッター
	void SetBossEnemy(BossEnemy* boss) { boss_ = boss; }

	void DrawImGui();

private:

	// HPによってスコアを加算する
	void BossHpScore(const BossStateScoreData& data);

	// 距離によってスコアを加算する
	void DistanceScore(const BossStateScoreData& data, BossState state, float distance);

	// 見られていたかによってスコアを加算する
	void LookingScore(const BossStateScoreData& data);

	// スコアが超えていたらステートを変化させる
	void ScoreChangeToState();


private:

	// ボス本体のポインタ
	BossEnemy* boss_ = nullptr;

	// ボスの新しいステート管理用の配列
	std::list<BossState> newBossStates_;

	// 更新用のタイム
	float timer_ = 0.0f;

	// 今の時間内で敵が見ていたか保存する
	bool isLook_ = false;

	// ステートごとのスコア
	uint32_t jumpAttackScore_ = 0;
	uint32_t dashAttackScore_ = 0;
	uint32_t spawnScore_ = 0;

};

