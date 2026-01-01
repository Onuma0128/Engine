#pragma once

#include <memory>

#include "Objects/Character/BaseCharacter.h"
#include "SearchAlgorithm/Navigation/PathFinder.h"

#include "Objects/Boss/AdjustItem/BossAdjustItem.h"
#include "Objects/Boss/State/BossBaseState.h"
#include "Objects/Boss/Collider/BossAttackCollider.h"
#include "Objects/Boss/Evaluator/BossStateEvaluator.h"
#include "Objects/Boss/Effect/BossEffect.h"
#include "Objects/Boss/Ray/BossRay.h"

// 前方宣言
class Player;
class MapCollision;
class EnemySpawnerFactory;

/// <summary>
/// ボス敵のクラス
/// </summary>
class BossEnemy : public BaseCharacter
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// ステートを変える
	/// </summary>
	/// <param name="newState"></新しいステートを入れる>
	void ChangeState(std::unique_ptr<BossBaseState> newState);

	/// <summary>
	/// 経路探索をリセットする
	/// </summary>
	void ResetSearch(const Vector3& goalPosition);

	/// <summary>
	/// ボスがスタートする
	/// </summary>
	void StartBossEnemy();

private:

	/// <summary>
	/// 当たり判定
	/// </summary>
	/// <param name="other"></当たったColliderのポインタが入る>
	void OnCollisionEnter(Collider* other) override;
	void OnCollisionStay(Collider* other) override;
	void OnCollisionExit(Collider* other) override;

public:

	// セッター
	void SetMapData(MapCollision* mapData) { pathFinder_.SetMapData(mapData); }
	void SetPlayer(Player* player) { player_ = player; }
	void SetEnemySpawnerFactory(EnemySpawnerFactory* factory) { spawnerFactory_ = factory; }

	// ゲッター
	const bool GetLooking()const { return ray_->GetLooking(); }
	const uint32_t GetCurrentHp()const { return currentHp_; }
	const Player* GetPlayer()const { return player_; }
	const BossAdjustItem* GetItems() const { return items_.get(); }
	const BossState GetBossState()const { return state_->GetState(); }
	BossStateEvaluator* GetStateEvaluator() { return stateEvaluator_.get(); }
	EnemySpawnerFactory* GetSpawnerFactory() { return spawnerFactory_; }
	BossAttackCollider* GetAttackCollider() { return attackCollider_.get(); }
	BossEffect* GetEffect() { return effect_.get(); }
	PathFinder& GetPathFinder() { return pathFinder_; }

private:
	
	// プレイヤーのポインタ
	Player* player_ = nullptr;

	// 敵スポナー管理クラスのポインタ
	EnemySpawnerFactory* spawnerFactory_ = nullptr;

	// ボスの調整項目
	std::unique_ptr<BossAdjustItem> items_ = nullptr;

	// ボスの状態遷移用ステート
	std::unique_ptr<BossBaseState> state_ = nullptr;
	std::unique_ptr<BossStateEvaluator> stateEvaluator_ = nullptr;

	// ボスの攻撃用コライダー
	std::unique_ptr<BossAttackCollider> attackCollider_ = nullptr;

	// ボスのエフェクト
	std::unique_ptr<BossEffect> effect_ = nullptr;

	// ボスの目線
	std::unique_ptr<BossRay> ray_ = nullptr;

	// 経路探索
	PathFinder pathFinder_;

	// ボスのMaxHP
	uint32_t maxHp_ = 1;
	// ボスの現在のHP
	uint32_t currentHp_ = 1;

};

