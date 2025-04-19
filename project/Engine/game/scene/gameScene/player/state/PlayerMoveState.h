#pragma once

#include "PlayerBaseState.h"

class PlayerMoveState : public PlayerBaseState
{
public:

	PlayerMoveState(Player* player);

	void Init()override;

	void Finalize()override;

	void Update()override;

	void Draw()override;

private:

	void ReloadBullet();

private:

	// 今弾をリロードしているか
	bool isReloadBullet_ = false;
	// 一つの弾をリロードする時間
	float reloadBulletTime_;

};

