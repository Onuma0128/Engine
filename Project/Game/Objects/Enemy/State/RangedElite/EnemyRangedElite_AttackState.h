#pragma once

#include "objects/enemy/state/EnemyBaseState.h"

/// <summary>
/// 強遠距離敵の攻撃ステートクラス
/// </summary>
class EnemyRangedElite_AttackState : public EnemyBaseState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy"></param>
	EnemyRangedElite_AttackState(BaseEnemy* enemy);

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
	/// 攻撃処理
	/// </summary>
	void Attack();

private:

	float chengeStateTime_ = 0.0f;

	bool isAttack_ = false;

};

