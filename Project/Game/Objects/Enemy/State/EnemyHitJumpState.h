#pragma once

#include "EnemyBaseState.h"

#include "Vector3.h"

class EnemyHitJumpState : public EnemyBaseState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy"></param>
	EnemyHitJumpState(BaseEnemy* enemy);

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
	/// Y座標設定
	/// </summary>
	/// <param name="y"></param>
	void SetTranslate(float y, const Vector3& velocity);

private:

	// 開始Y座標
	float startY_ = 0.0f;

	// ジャンプ関連パラメータ
	float hitJumpVelocityY_ = 0.0f;
	float hitJumpAccelerY_ = 0.0f;
	float hitJumpNockbackScale_ = 0.0f;


};

