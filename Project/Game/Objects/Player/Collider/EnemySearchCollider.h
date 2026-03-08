#pragma once

#include "Collider.h"

#include "Vector3.h"

// 前方宣言
class Player;

/// <summary>
/// 敵探索用のコライダークラス
/// </summary>
class EnemySearchCollider : public Collider
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
	const Vector3& GetTargetPosition()const { return targetPosition_; }

	// アクセッサ
	void SetPlayer(Player* player) { player_ = player; }

	Collider* GetTargetCollider() { return targetCollider_; }
	const bool GetIsHit()const { return isHit_ ; }

private:

	/// <summary>
	/// 当たり判定
	/// </summary>
	/// <param name="other"></当たったColliderのポインタが入る>
	void OnCollisionEnter(Collider* other) override;
	void OnCollisionStay(Collider* other) override;
	void OnCollisionExit(Collider* other) override;


	/// <summary>
	/// ターゲットの情報をリセットする
	/// </summary>
	void ResetTarget();

private:

	// プレイヤーのポインタ
	Player* player_ = nullptr;

	// ターゲットした時のコライダー
	Collider* targetCollider_ = nullptr;

	// 一番近い敵の座標
	Vector3 targetPosition_;

	// 一番近い敵との距離
	float targetDistance_;

	// 敵と当たっているか
	bool isHit_ = false;
};

