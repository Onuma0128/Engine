#pragma once

#include "EnemyBaseState.h"

#include "Vector3.h"

class EnemyKnockbackState : public EnemyBaseState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy"></param>
	EnemyKnockbackState(BaseEnemy* enemy);

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

	float time_ = 0.0f;
	float speed_ = 0.0f;

	Vector3 startPosition_;
	Vector3 endPosition_;

};

