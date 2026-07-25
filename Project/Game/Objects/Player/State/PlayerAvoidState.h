#pragma once

#include "PlayerBaseState.h"

#include "Vector3.h"
#include "Quaternion.h"

/// <summary>
/// プレイヤーの回避状態
/// </summary>
class PlayerAvoidState :public PlayerBaseState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="player"></param>
	PlayerAvoidState(Player* player);

	/// <summary>
	/// 現在のステートを取得する
	/// </summary>
	/// <returns></returns>
	PlayerState GetState()const override { return PlayerState::Avoid; }

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

	/// <summary>
	/// 回避時の移動ベクトルを生成する
	/// </summary>
	/// <returns></returns>
	const NumaEngine::Vector3 CreateVelocity();

private:

	float avoidTime_ = 0.0f;

	NumaEngine::Vector3 velocity_ = {};
	float velocityY_ = 0.0f;
	float acceleration_ = 0.0f;
	NumaEngine::Quaternion rotateY_ = NumaEngine::Quaternion::IdentityQuaternion();

};



