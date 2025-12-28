#pragma once

// 前方宣言
class MuscleCompanion;

// キャラクターステートの列挙型
enum class CharacterState {
	Idle,
	Move,
	Dash,
	Attack,
	Dead,
};

/// <summary>
/// 仲間の基底ステートクラス
/// </summary>
class CompanionBaseState
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~CompanionBaseState() = default;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="player"></param>
	CompanionBaseState(MuscleCompanion* companion);

	/// <summary>
	/// 現在のステートを取得する
	/// </summary>
	/// <returns></returns>
	virtual CharacterState GetState() const = 0;

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

	MuscleCompanion* companion_ = nullptr;

};

