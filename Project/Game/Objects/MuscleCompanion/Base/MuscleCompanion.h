#pragma once

#include <memory>

#include "Objects/Character/BaseCharacter.h"
#include "SearchAlgorithm/Navigation/PathFinder.h"

#include "Objects/MuscleCompanion/State/CompanionBaseState.h"
#include "Objects/MuscleCompanion/AdjustItem/CompanionAdjustItem.h"
#include "Objects/MuscleCompanion/Collider/CompanionAttackCollider.h"

// 前方宣言
class Player;
class MapCollision;

/// <summary>
/// プレイヤー仲間の筋肉クラス
/// </summary>
class MuscleCompanion : public BaseCharacter
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

private:

	/// <summary>
	/// 当たり判定
	/// </summary>
	/// <param name="other"></当たったColliderのポインタが入る>
	void OnCollisionEnter(Collider* other) override;
	void OnCollisionStay(Collider* other) override;
	void OnCollisionExit(Collider* other) override;

public:

	/// <summary>
	/// ステートを変える
	/// </summary>
	/// <param name="newState"></新しいステートを入れる>
	void ChangeState(std::unique_ptr<CompanionBaseState> newState);

	/// <summary>
	/// 経路探索をリセットする
	/// </summary>
	void ResetSearch();

	/// <summary>
	/// プレイヤーとの距離を測りフラグを返す
	/// </summary>
	/// <returns></returns>
	bool SearchDistance();

	// セッター
	void SetPlayer(Player* player) { player_ = player; }
	void SetItems(CompanionAdjustItem* items) { items_ = items; }
	void SetMapData(MapCollision* mapData) { pathFinder_.SetMapData(mapData); }
	void SetGatherRequested(bool flag) { isGatherRequested_ = flag; }
	void SetDashColliderScale(float scale) { dashColliderScale_ = scale; }
	void SetFirstDashAttack(bool flag) { isFirstDashAttack_ = flag; }
	// ゲッター
	const CompanionAdjustItem* GetItems()const { return items_; }
	const Player* GetPlayer()const { return player_; }
	const CharacterState GetState() const { return state_->GetState(); }
	CompanionAttackCollider* GetAttackCollider() { return attackCollider_.get(); }
	bool GetGatherRequested() const { return isGatherRequested_; }
	bool GetFirstDashAttack() const { return isFirstDashAttack_; }
	PathFinder& GetPathFinder() { return pathFinder_; }

private:

	// プレイヤーのポインタ
	Player* player_ = nullptr;
	// 調整項目のポインタ
	CompanionAdjustItem* items_ = nullptr;
	// 状態遷移
	std::unique_ptr<CompanionBaseState> state_ = nullptr;
	// 攻撃用コライダー
	std::unique_ptr<CompanionAttackCollider> attackCollider_ = nullptr;
	// 経路探索
	PathFinder pathFinder_;

	// 仲間のMaxHP
	uint32_t maxHp_ = 1;
	// 仲間の現在のHP
	uint32_t currentHp_ = 1;
	// 集合要求フラグ
	bool isGatherRequested_ = false;
	// ダッシュ時のコライダーのスケール
	float dashColliderScale_ = 1.0f;
	// ダッシュ時の一回目の攻撃かどうか
	bool isFirstDashAttack_ = true;
};

