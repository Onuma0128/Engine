#pragma once

#include "PlayerBaseState.h"

#include "Vector3.h"
#include "Quaternion.h"

class PlayerMoveState : public PlayerBaseState
{
public:

	PlayerMoveState(Player* player);

	void Init()override;

	void Finalize()override;

	void Update()override;

	void Draw()override;

private:
	void SomeAction();
	void ReloadBullet();

	const Vector3 CreateMoveVelocity();
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

