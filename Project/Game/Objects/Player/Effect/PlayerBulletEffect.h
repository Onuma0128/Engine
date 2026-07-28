#pragma once

#include <array>

#include "ParticleManager.h"
#include "ParticleEmitter.h"

#include "WorldTransform.h"

/// <summary>
/// プレイヤーの弾のエフェクトを管理するクラス
/// </summary>
class PlayerBulletEffect
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
	/// 弾のトレイルエフェクトを呼び出す
	/// </summary>
    void OnceBulletTrailEffect(const NumaEngine::WorldTransform& transform);

	/// <summary>
	/// 弾が消える時のエフェクトを呼び出す
	/// </summary>
   void OnceBulletDeleteEffect(const NumaEngine::WorldTransform& transform);

	/// <summary>
	/// 弾のエフェクトを呼び出す
	/// </summary>
 void OnceBulletEffect(const NumaEngine::WorldTransform& transform);

	/// <summary>
	/// 弾リロード時のエフェクトを呼び出す
	/// </summary>
   void OnceBulletReloadEffect(const NumaEngine::WorldTransform& transform);

private:

    // パーティクルを管理する
	NumaEngine::ParticleManager* particleManager_ = NumaEngine::ParticleManager::GetInstance();

	/* ==================== プレイヤーが弾を撃った時のエフェクト ==================== */

    // 弾のトレイルエフェクト
	std::shared_ptr<NumaEngine::ParticleEmitter> bulletTrailEmitter_;
	// 弾が消える時のエフェクト
	std::shared_ptr<NumaEngine::ParticleEmitter> bulletDeleteEmitter_;

	/* ==================== プレイヤーが弾を撃った時のエフェクト ==================== */

    // 弾を撃った時の爆発
	std::shared_ptr<NumaEngine::ParticleEmitter> bulletExplosionEmitter_ = nullptr;
	// 弾を撃った時の火花
	std::shared_ptr<NumaEngine::ParticleEmitter> bulletSparkEmitter_ = nullptr;
	// 弾を撃った後の煙
	std::shared_ptr<NumaEngine::ParticleEmitter> bulletSmokeEmitter_ = nullptr;
	// 弾を撃った後の薬莢
	std::shared_ptr<NumaEngine::ParticleEmitter> bulletCartridgeEmitter_ = nullptr;
};

