#pragma once
#include <memory>
#include <array>
#include <vector>

#include "Collision3D.h"
#include "SceneJsonLoader.h"
#include "JsonFunction.h"

#include "Objects/Character/BaseCharacter.h"
#include "objects/player/state/PlayerBaseState.h"
#include "objects/player/effect/PlayerEffect.h"
#include "objects/player/reticle/PlayerReticle.h"
#include "objects/player/adjustItem/PlayerAdjustItem.h"
#include "objects/player/bullet/PlayerShot.h"

/// <summary>
/// プレイヤー
/// </summary>
class Player : public BaseCharacter
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// </summary>
	/// エフェクト描画
	/// </summary>
	void EffectDraw();

	/// <summary>
	/// ステートを変える
	/// </summary>
	/// <param name="newState"></新しいステートを入れる>
	void ChangeState(std::unique_ptr<PlayerBaseState> newState);

private:

	/// <summary>
	/// 当たり判定
	/// </summary>
	/// <param name="other"></当たったColliderのポインタが入る>
	void OnCollisionEnter(Collider* other) override;
	void OnCollisionStay(Collider* other) override;
	void OnCollisionExit(Collider* other) override;

public:

	/// ==================== アクセッサ ==================== ///

	PlayerShot* GetShot() const { return shot_.get(); }
	PlayerEffect* GetEffect()const { return effect_.get(); }
	PlayerReticle* GetReticle()const { return reticle_.get(); }
	PlayerAdjustItem* GetItem() const { return items_.get(); }
	void SetLoader(SceneJsonLoader* loader) { loader_ = loader; }

	const WorldTransform& GetTransform() const { return transform_; }
	bool GetIsAlive()const { return isAlive_; }
	bool GetIsPlayingMouse()const { return isPlayingMouse_; }
	float GetAvoidCoolTimer()const { return avoidCoolTimer_; }
	bool GetIsPushMove()const { return isPushMove_; }

	void SetIsPlayingMouse(bool flag) { isPlayingMouse_ = flag; }
	void SetIsAvoid(bool flag) { isAvoid_ = flag; }
	void SetAvoidCoolTimer(float time) { avoidCoolTimer_ = time; }

private:

	// 状態遷移
	std::unique_ptr<PlayerBaseState> state_ = nullptr;
	// 弾
	std::unique_ptr<PlayerShot> shot_ = nullptr;
	// エフェクト
	std::unique_ptr<PlayerEffect> effect_ = nullptr;
	// 必殺技時にロックオンするためのレティクル
	std::unique_ptr<PlayerReticle> reticle_ = nullptr;
	// 調整項目
	std::unique_ptr<PlayerAdjustItem> items_;
	// ローダーのポインタ
	SceneJsonLoader* loader_ = nullptr;

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