#pragma once

#include "objects/enemy/state/EnemyBaseState.h"

/// <summary>
/// シールド持ち近接敵の攻撃ステートクラス
/// </summary>
class EnemyShieldBearer_AttackState : public EnemyBaseState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy"></param>
	EnemyShieldBearer_AttackState(BaseEnemy* enemy);

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

	float chengeStateTime_ = 0.0f;

	bool chengeAniamtion_ = false;

};

