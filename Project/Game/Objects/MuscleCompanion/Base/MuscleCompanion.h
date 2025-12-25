#pragma once

#include <memory>

#include "Objects/Character/BaseCharacter.h"
#include "SearchAlgorithm/Navigation/PathFinder.h"

#include "Objects/MuscleCompanion/State/CompanionBaseState.h"
#include "Objects/MuscleCompanion/AdjustItem/CompanionAdjustItem.h"

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
	// ゲッター
	const CompanionAdjustItem* GetItems()const { return items_; }
	const Player* GetPlayer()const { return player_; }
	const CharacterState GetState() const { return state_->GetState(); }
	bool GetGatherRequested() const { return isGatherRequested_; }
	PathFinder& GetPathFinder() { return pathFinder_; }

private:

	// プレイヤーのポインタ
	Player* player_ = nullptr;

	// 調整項目のポインタ
	CompanionAdjustItem* items_ = nullptr;

	// 状態遷移
	std::unique_ptr<CompanionBaseState> state_ = nullptr;

	// 集合要求フラグ
	bool isGatherRequested_ = false;

	// 経路探索
	PathFinder pathFinder_;

};

