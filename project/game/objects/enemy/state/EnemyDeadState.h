#pragma once

#include <random>

#include "EnemyBaseState.h"

#include "Vector3.h"
#include "Quaternion.h"

class EnemyDeadState : public EnemyBaseState
{
public:

	EnemyDeadState(BaseEnemy* enemy);

	void GlobalInit();

	void Init()override;

	void Finalize()override;

	void Update()override;

	void Draw()override;

private:

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

