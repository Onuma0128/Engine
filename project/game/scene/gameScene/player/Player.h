#pragma once
#include <memory>
#include <array>
#include <vector>

#include "BaseUI.h"
#include "Animation.h"
#include "Collider.h"
#include "Collision3D.h"
#include "SceneJsonLoader.h"
#include "JsonFunction.h"

#include "gameScene/player/state/PlayerBaseState.h"
#include "gameScene/player/effect/PlayerEffect.h"
#include "gameScene/player/reticle/PlayerReticle.h"
#include "gameScene/player/adjustItem/PlayerAdjustItem.h"

#include "gameScene/player/bullet/PlayerShot.h"

class Player : public Animation,Collider
{
public:

	void Init(SceneJsonLoader loader);

	void Update();

	void EffectDraw();
	void Draw();

	void ChengeState(std::unique_ptr<PlayerBaseState> newState);

	void OnCollisionEnter(Collider* other) override;
	void OnCollisionStay(Collider* other) override;
	void OnCollisionExit(Collider* other) override;

	PlayerShot* GetShot() { return shot_.get(); }
	PlayerEffect* GetEffect() { return effect_.get(); }
	PlayerReticle* GetReticle() { return reticle_.get(); }
	PlayerAdjustItem* GetItem() const { return items_.get(); }

	bool GetIsAlive()const { return isAlive_; }
	bool GetIsPlayingMouse()const { return isPlayingMouse_; }
	void SetIsPlayingMouse(bool flag) { isPlayingMouse_ = flag; }
	void SetIsAvoid(bool flag) { isAvoid_ = flag; }
	float GetAvoidCoolTimer()const { return avoidCoolTimer_; }
	void SetAvoidCoolTimer(float time) { avoidCoolTimer_ = time; }
	bool GetIsPushMove()const { return isPushMove_; }

private:

	// 状態遷移
	std::unique_ptr<PlayerBaseState> state_ = nullptr;
	// 弾
	std::unique_ptr<PlayerShot> shot_ = nullptr;
	// エフェクト
	std::unique_ptr<PlayerEffect> effect_ = nullptr;
	// 必殺技時にロックオンするためのレティクル
	std::unique_ptr<PlayerReticle> reticle_ = nullptr;
	// 操作系のUI
	std::vector<std::unique_ptr<BaseUI>> controlUIs_;
	// 調整項目
	std::unique_ptr<PlayerAdjustItem> items_;

	// 今生きているか
	bool isAlive_ = true;
	// 今避けているのか
	bool isAvoid_ = false;
	// 次の避けを行える時間
	float avoidCoolTimer_ = 0.0f;
	// 押し出し処理をしているか
	bool isPushMove_ = false;
	// マウスを使ってプレイしているか
	bool isPlayingMouse_ = false;

};