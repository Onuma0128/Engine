#pragma once

#include "PlayerBaseState.h"

#include "Vector3.h"
#include "Quaternion.h"

/// <summary>
/// プレイヤーの移動状態
/// </summary>
class PlayerMoveState : public PlayerBaseState
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="player"></param>
	PlayerMoveState(Player* player);

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
	/// 一定のアクションを実行する
	/// </summary>
	void SomeAction();

	/// <summary>
	/// 弾をリロードする
	/// </summary>
	void ReloadBullet();

	/// <summary>
	/// 移動時の速度ベクトルを生成する
	/// </summary>
	/// <returns></returns>
	const Vector3 CreateMoveVelocity();

	/// <summary>
	/// 回転時の速度ベクトルを生成する
	/// </summary>
	/// <returns></returns>
	const Vector3 CreateRotateVelocity();

private:

	// 今弾をリロードしているか
	bool isReloadBullet_ = false;
	// リロードをはじめる時間
	float reloadStartTime_ = 0.0f;
	// 一つの弾をリロードする時間
	float reloadBulletTime_;

	// 最後に向けたRightStickを保存する
	Vector3 rightStickVelocity_{};
	Quaternion rightStickQuaternion_{};
	bool chengeAniamtion_ = false;
	bool isReversePlay_ = false;

};

