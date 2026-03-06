#pragma once

#include "Objects/MuscleCompanion/State/CompanionBaseState.h"

class CompanionPushUpIdleState : public CompanionBaseState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="player"></param>
	CompanionPushUpIdleState(MuscleCompanion* companion);

	/// <summary>
	/// 現在のステートを取得する
	/// </summary>
	/// <returns></returns>
	CharacterState GetState() const override { return CharacterState::PushUpIdle; }

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

	float pushUpTime_ = 0.0f;

	bool isPowerUp_ = false;

};

