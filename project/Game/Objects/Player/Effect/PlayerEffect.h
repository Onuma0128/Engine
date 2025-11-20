#pragma once
#include <array>

#include "ParticleManager.h"
#include "ParticleEmitter.h"
#include "PrimitiveDrawr.h"

#include "WorldTransform.h"

/// <summary>
/// 必殺技の状態
/// </summary>
enum class SpecialMoveState {
	kNone,
	kExpanding,   // 拡大中（0→1）
	kHolding,     // 維持（1）
	kShrinking    // 縮小中（1→0）
};

class Player;

/// <summary>
/// プレイヤーのエフェクトを管理するクラス
/// </summary>
class PlayerEffect
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PlayerEffect();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// プレイヤーを設定する
	/// </summary>
	/// <param name="player"></param>
	void SetPlayer(Player* player) { player_ = player; }

	/// <summary>
	/// 移動時のエフェクトを呼び出す
	/// </summary>
	void OnceMoveEffect();

	/// <summary>
	/// 避けた時のエフェクトを呼び出す
	/// </summary>
	void OnceAvoidEffect();

	/// <summary>
	/// 攻撃を受けた時のエフェクトを呼び出す
	/// </summary>
	void OnceHitEffect();

	/// <summary>
	/// 必殺技を撃った時のエフェクト更新
	/// </summary>
	void UpdatePostEffect();

	/// <summary>
	/// 必殺技の状態を取得する
	/// </summary>
	/// <returns></returns>
	SpecialMoveState GetSpecialState()const { return specialMoveState_; }

	/// <summary>
	/// 必殺技のフレームを取得する
	/// </summary>
	/// <returns></returns>
	float GetSpecialMoveFrame()const { return specialMoveFrame_; }

	/// <summary>
	/// 必殺技の発動中かどうかを取得する
	/// </summary>
	/// <returns></returns>
	bool GetIsSpecialMove()const { return isSpecialMove_; }

	/// <summary>
	/// 必殺技の発動中かどうかを設定する
	/// </summary>
	/// <param name="flag"></param>
	void SetIsSpecialMove(bool flag) { isSpecialMove_ = flag; }

	/// <summary>
	/// リロードUIの描画を有効/無効にする
	/// </summary>
	/// <param name="flag"></param>
	void SetIsDrawReloadUI(bool flag) { playerReload_->GetRenderOptions().enabled = flag; }

private:

	Player* player_ = nullptr;

	// パーティクルを管理する
	ParticleManager* particleManager_ = ParticleManager::GetInstance();

	/* ==================== プレイヤーが移動時のエフェクト ==================== */

	// 移動時の土埃
	std::shared_ptr<ParticleEmitter> moveDustEmitter_ = nullptr;

	/* ==================== プレイヤーが避けた時のエフェクト ==================== */

	// 避けた時の土埃
	std::shared_ptr<ParticleEmitter> avoidDustEmitter_ = nullptr;

	/* ==================== プレイヤーがリロードした時のエフェクト ==================== */
	
	std::unique_ptr<PrimitiveDrawr> playerReload_ = nullptr;

	/* ==================== プレイヤーが必殺技を撃った時のエフェクト ==================== */

	// シリンダーで必殺の範囲を視覚的に
	std::unique_ptr<PrimitiveDrawr> cylinder_ = nullptr;
	// 必殺技の遷移状態
	SpecialMoveState specialMoveState_ = SpecialMoveState::kNone;
	// 今必殺技を撃っているか
	bool isSpecialMove_ = false;
	// 必殺技のフレーム
	float specialMoveFrame_ = 0.0f;

	/* ==================== プレイヤーが攻撃を受けた時のエフェクト ==================== */

	std::shared_ptr<ParticleEmitter> playerHit_ = nullptr;
};

