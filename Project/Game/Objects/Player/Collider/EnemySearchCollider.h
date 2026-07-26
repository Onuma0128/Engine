#pragma once

#include "Collider.h"

#include "Vector3.h"
#include <cfloat>

// 前方宣言
class Player;

/// <summary>
/// 敵探索用のコライダークラス
/// </summary>
class EnemySearchCollider : public NumaEngine::Collider
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
	/// 一番近い敵の座標を取得する
	/// </summary>
	/// <returns></returns>
	const NumaEngine::Vector3& GetTargetPosition()const { return targetPosition_; }

	// アクセッサ
	void SetPlayer(Player* player) { player_ = player; }

    NumaEngine::Collider* GetTargetCollider() { return targetCollider_; }
	const bool GetIsHit()const { return isHit_ ; }

private:

	/// <summary>
	/// 当たり判定
	/// </summary>
	/// <param name="other"></当たったColliderのポインタが入る>
    void OnCollisionEnter(NumaEngine::Collider* other) override;
	void OnCollisionStay(NumaEngine::Collider* other) override;
	void OnCollisionExit(NumaEngine::Collider* other) override;


	/// <summary>
	/// ターゲットの情報をリセットする
	/// </summary>
	void ResetTarget();

private:

	// プレイヤーのポインタ
	Player* player_ = nullptr;

	// ターゲットした時のコライダー
    NumaEngine::Collider* targetCollider_ = nullptr;

	// 一番近い敵の座標
	NumaEngine::Vector3 targetPosition_;

    // 一番近い敵との距離
	float targetDistance_ = FLT_MAX;

	// 敵と当たっているか
	bool isHit_ = false;
};



