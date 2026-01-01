#pragma once

// 前方宣言
class BossEnemy;

// キャラクターステートの列挙型
enum class BossState {
	Down,				// ダウン状態
	Move,				// 移動
	EnemySpawn,			// 敵スポーン
	Melee,				// 近接攻撃
	DashAttack,			// ダッシュ攻撃
	JumpAttack,			// ジャンプ攻撃
	Dead,				// 死亡
};

/// <summary>
/// ボスの基底ステートクラス
/// </summary>
class BossBaseState
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~BossBaseState() = default;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="boss"></param>
	BossBaseState(BossEnemy* boss);

	/// <summary>
	/// 現在のステートを取得する
	/// </summary>
	/// <returns></returns>
	virtual BossState GetState() const = 0;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() = 0;

	/// <summary>
	/// 終了
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

	BossEnemy* boss_ = nullptr;

};

