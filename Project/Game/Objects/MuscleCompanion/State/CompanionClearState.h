#pragma once

#include "Objects/MuscleCompanion/State/CompanionBaseState.h"

/// <summary>
/// クリア時のステートクラス
/// </summary>
class CompanionClearState : public CompanionBaseState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="player"></param>
	CompanionClearState(MuscleCompanion* companion);

	/// <summary>
	/// 現在のステートを取得する
	/// </summary>
	/// <returns></returns>
	CharacterState GetState() const override { return CharacterState::Clear; }

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
};

