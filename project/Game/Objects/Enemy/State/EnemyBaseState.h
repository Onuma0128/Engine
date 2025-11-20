#pragma once

class BaseEnemy;

/// <summary>
/// 敵の基底ステートクラス
/// </summary>
class EnemyBaseState
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyBaseState() = default;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="enemy"></param>
	EnemyBaseState(BaseEnemy* enemy);

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() = 0;

	/// <summary>
	/// 終了処理
	/// </summary>
	virtual void Finalize() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

protected:

	BaseEnemy* enemy_ = nullptr;

};
