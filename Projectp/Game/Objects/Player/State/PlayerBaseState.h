#pragma once

class Player;

/// <summary>
/// プレイヤーの基底ステートクラス
/// </summary>
class PlayerBaseState
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PlayerBaseState() = default;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="player"></param>
	PlayerBaseState(Player* player);

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

	Player* player_ = nullptr;

};