#pragma once

#include "ParticleManager.h"
#include "ParticleEmitter.h"

// 前方宣言
class MuscleCompanion;

/// <summary>
/// 仲間のパーティクル管理クラス
/// </summary>
class CompanionEffect
{
public:

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
	/// 仲間を設定する
	/// </summary>
	/// <param name="player"></param>
	void SetCompanion(MuscleCompanion* companion) { companion_ = companion; }

	/// <summary>
	/// 移動時のエフェクトを呼び出す
	/// </summary>
	void OnceMoveEffect();

	/// <summary>
	/// ダッシュ時のエフェクトを呼び出す
	/// </summary>
	void OnceDashEffect(bool isSearch);

	/// <summary>
	/// ヒット時のエフェクトを呼び出す
	/// </summary>
	void OnceHitEffect();

private:

	// 仲間のポインタ
	MuscleCompanion* companion_ = nullptr;

	// パーティクルを管理する
	ParticleManager* particleManager_ = ParticleManager::GetInstance();

	/* ==================== 仲間が移動時のエフェクト ==================== */

	// 移動時の土埃
	std::shared_ptr<ParticleEmitter> moveDustEmitter_ = nullptr;

	/* ==================== 仲間がダッシュ時のエフェクト ==================== */

	// 移動時の土埃
	std::shared_ptr<ParticleEmitter> dashEmitter_ = nullptr;
	std::shared_ptr<ParticleEmitter> searchDashEmitter_ = nullptr;

	/* ==================== 仲間がダメージ時のエフェクト ==================== */

	// 移動時の土埃
	std::shared_ptr<ParticleEmitter> hitEmitter_ = nullptr;

};

