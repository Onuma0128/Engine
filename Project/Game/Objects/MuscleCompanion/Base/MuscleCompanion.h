#pragma once

#include <memory>

#include "Audio.h"
#include "Objects/Character/BaseCharacter.h"
#include "SearchAlgorithm/Navigation/PathFinder.h"

#include "Objects/MuscleCompanion/State/CompanionBaseState.h"
#include "Objects/MuscleCompanion/AdjustItem/CompanionAdjustItem.h"
#include "Objects/MuscleCompanion/Collider/CompanionAttackCollider.h"
#include "Objects/MuscleCompanion/Collider/CompanionFollowerCollider.h"
#include "Objects/MuscleCompanion/Effect/CompanionEffect.h"

// 前方宣言
class Player;
class MapCollision;
class GameCamera;

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
	void ResetSearch(const Vector3& goalPosition);

	/// <summary>
	/// プレイヤーとの距離を測りフラグを返す
	/// </summary>
	/// <returns></returns>
	bool SearchDistance();

	// セッター
	void SetPlayer(Player* player) { player_ = player; }
	void SetItems(CompanionAdjustItem* items) { items_ = items; }
	void SetMapData(MapCollision* mapData) { pathFinder_.SetMapData(mapData); }
	void SetCamera(GameCamera* camera) { camera_ = camera; }
	void SetAudio(Audio* audio) { audio_ = audio; }
	void SetColliderIsActive(bool flag) { isActive_ = flag; }
	void SetColliderScale(float scale) { colliderScale_ = scale; }
	void SetGatherRequested(bool flag) { isGatherRequested_ = flag; }
	void SetReturnOriginal(bool flag) { isReturnOriginal_ = flag; }
	void SetFirstDashAttack(bool flag) { isFirstDashAttack_ = flag; }
	// ゲッター
	const CompanionAdjustItem* GetItems()const { return items_; }
	const Player* GetPlayer()const { return player_; }
	const CharacterState GetState() const { return state_->GetState(); }
	CompanionAttackCollider* GetAttackCollider() { return attackCollider_.get(); }
	CompanionFollowerCollider* GetFollowerCollider() { return followerCollider_.get(); }
	CompanionEffect* GetEffect() { return effect_.get(); }
	GameCamera* GetCamera() { return camera_; }
	Audio* GetAudio() { return audio_; }
	PathFinder& GetPathFinder() { return pathFinder_; }
	bool GetGatherRequested() const { return isGatherRequested_; }
	bool GetReturnOriginal() const { return isReturnOriginal_; }
	bool GetFirstDashAttack() const { return isFirstDashAttack_; }
	const Vector3& GetKnockbackDire()const { return knockbackDirection_; }

private:

	// プレイヤーのポインタ
	Player* player_ = nullptr;
	// 調整項目のポインタ
	CompanionAdjustItem* items_ = nullptr;
	// ゲームカメラ
	GameCamera* camera_ = nullptr;
	// オーディオ
	Audio* audio_ = nullptr;
	// 状態遷移
	std::unique_ptr<CompanionBaseState> state_ = nullptr;
	// 攻撃用コライダー
	std::unique_ptr<CompanionAttackCollider> attackCollider_ = nullptr;
	// 後続用コライダー
	std::unique_ptr<CompanionFollowerCollider> followerCollider_ = nullptr;
	// エフェクト
	std::unique_ptr<CompanionEffect> effect_ = nullptr;
	// 経路探索
	PathFinder pathFinder_;

	// 仲間のMaxHP
	uint32_t maxHp_ = 1;
	// 仲間の現在のHP
	uint32_t currentHp_ = 1;
	// コライダーのスケール
	float colliderScale_ = 1.0f;
	// 集合要求フラグ
	bool isGatherRequested_ = false;
	// 元の場所に戻ったか
	bool isReturnOriginal_ = true;
	// ダッシュ時の一回目の攻撃かどうか
	bool isFirstDashAttack_ = true;
	// ノックバック方向
	Vector3 knockbackDirection_ = {};
};

