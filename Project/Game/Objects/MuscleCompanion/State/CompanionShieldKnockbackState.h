#pragma once

#include "Objects/MuscleCompanion/State/CompanionBaseState.h"

#include "Vector3.h"

/// <summary>
/// 敵のシールドに弾かれた際のノックバックステートクラス
/// </summary>
class CompanionShieldKnockbackState : public CompanionBaseState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="player"></param>
	CompanionShieldKnockbackState(MuscleCompanion* companion);

	/// <summary>
	/// 現在のステートを取得する
	/// </summary>
	/// <returns></returns>
	CharacterState GetState() const override { return CharacterState::ShieldKnockback; }

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

	// 時間
	float timer_ = 0.0f;
	float maxTime_ = 0.0f;

	// ノックバックされる座標
	Vector3 prePos_ = {};
	Vector3 target_ = {};

};

