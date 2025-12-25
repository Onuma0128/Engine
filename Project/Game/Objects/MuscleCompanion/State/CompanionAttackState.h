#pragma once

#include "Objects/MuscleCompanion/State/CompanionBaseState.h"

#include "Vector3.h"
#include "Quaternion.h"

class CompanionAttackState : public CompanionBaseState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="player"></param>
	CompanionAttackState(MuscleCompanion* companion);

	/// <summary>
	/// 現在のステートを取得する
	/// </summary>
	/// <returns></returns>
	CharacterState GetState() const override { return CharacterState::Attack; }

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

	// 攻撃の速度ベクトル
	Vector3 velocity_ = Vector3::ExprZero;


	Quaternion yRotation_ = Quaternion::IdentityQuaternion();

};

