#pragma once

#include <random>

#include "EnemyBaseState.h"

#include "Vector3.h"
#include "Quaternion.h"

/// <summary>
/// 敵の死亡ステートクラス
/// </summary>
class EnemyDeadState : public EnemyBaseState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy"></param>
	EnemyDeadState(BaseEnemy* enemy);

	/// <summary>
	/// グローバル初期化
	/// </summary>
	void GlobalInit();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init()override;

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize()override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;

private:

	/// <summary>
	/// 結果のターゲットオフセットを計算する
	/// </summary>
	void ResultTargetOffset();

private:

	// 死亡までの時間
	float maxDeadTimer_ = 0.0f;
	// 死亡までの経過時間
	float deadTimer_ = 0.0f;
	// ノックバックしている時間
	float knockbackTimer_ = 0.0f;
	// ノックバック時にスケールが変化している時間
	float kNockbackScaleTimer_ = 0.0f;
	// パーティクルが出ている時間
	float particleTimer_ = 0.0f;
	// 敵から見て弾がどの方向に当たったか
	bool isLeft_ = false;
	// 敵が死ぬ時のvelocityとacceleration
	float velocityY_ = 0.0;
	float accelerationY_ = 0.0;
	float kNockbackScale_ = 0.0f;
	// アニメーションを変える
	bool chengeAnimation_ = false;

	// ノックバックする方向
	Vector3 velocity_{};
	// ノックバックした座標
	Vector3 target_{};
	Quaternion targetRotate_{};
	// 死亡時のScaleを取得
	Vector3 defaultScale_{};
	// 死亡時の回転を取得
	Quaternion defaultRotate_ = Quaternion::IdentityQuaternion();
	// ランダムデバイス
	std::random_device seedGenerator_;

};

