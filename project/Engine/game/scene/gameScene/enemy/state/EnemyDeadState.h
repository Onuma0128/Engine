#pragma once

#include "EnemyBaseState.h"

#include "Vector3.h"
#include "WorldTransform.h"

class EnemyDeadState : public EnemyBaseState
{
public:

	EnemyDeadState(Enemy* enemy);

	void GlobalInit();

	void Init()override;

	void Finalize()override;

	void Update()override;

	void Draw()override;

private:

	// 死亡までの時間
	float maxDeadFrame_ = 0.0f;
	// 死亡までの経過時間
	float deadFrame_ = 0.0f;
	// ノックバックしている時間
	float knockbackFrame_ = 0.0f;
	// パーティクルが出ている時間
	float particleFrame_ = 0.0f;

	// ノックバックする方向
	Vector3 velocity_{};
	// ノックバックした座標
	Vector3 target_{};
	// 死亡時のScaleを取得
	Vector3 defaultScale_{};

};

