#pragma once

#include "BossBaseState.h"

// 登場時のステート
enum class AppearState
{
	StartupTime,	// 攻撃開始までの硬直
	fallDownTime,	// 落下している時間
	Recover,		// 着地後の硬直
	Finish,			// 終了
};

/// <summary>
/// 登場時のステートクラス
/// </summary>
class BossAppearState : public BossBaseState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="boss"></param>
	BossAppearState(BossEnemy* boss);

	/// <summary>
	/// 現在のステートを取得する
	/// </summary>
	/// <returns></returns>
	BossState GetState() const override { return BossState::Appear; }

	/// <summary>
	/// 初期化
	/// </summary>
	void Init()override;

	/// <summary>
	/// 終了
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

	void ChangeAppearState(AppearState newState);

	void UpdateJump();

private:

	// 登場タイマー
	float timer_ = 0.0f;

	// 登場ステート
	AppearState appearState_ = AppearState::StartupTime;

	// 開始Y座標
	float startY_ = 0.0f;

	// ジャンプ関連パラメータ
	float jumpVelocityY_ = 0.0f;
	float jumpAccelerY_ = 0.0f;

};

