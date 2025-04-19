#pragma once
#include <memory>
#include <array>

#include "Object3d.h"

#include "gameScene/player/state/PlayerBaseState.h"
#include "gameScene/player/effect/PlayerEffect.h"
#include "gameScene/player/bullet/PlayerBullet.h"
#include "gameScene/player/ui/PlayerBulletUI.h"

class Player : public Object3d
{
public:

	void Init();

	void GlobalInit();

	void Update();

	void Draw();


	void ChengeState(std::unique_ptr<PlayerBaseState> newState);

	// 弾のリロードを管理する関数
	void ReloadBullet();

	// 弾を発射する関数
	void AttackBullet();

	PlayerEffect* GetEffect() { return effect_.get(); }

private:

	void BulletInit();


private:

	// 状態遷移
	std::unique_ptr<PlayerBaseState> state_ = nullptr;

	// エフェクト
	std::unique_ptr<PlayerEffect> effect_ = nullptr;

	// 弾を6つ生成する
	std::array<std::unique_ptr<PlayerBullet>, 6> bullets_;
	std::array<std::unique_ptr<PlayerBulletUI>, 6> bulletUIs_;

};