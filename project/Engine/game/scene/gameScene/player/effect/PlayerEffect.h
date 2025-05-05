#pragma once
#include <array>

#include "ParticleManager.h"
#include "ParticleEmitter.h"

#include "WorldTransform.h"

enum class GrayscaleState {
	None,
	Expanding,   // 拡大中（0→1）
	Holding,     // 維持（1）
	Shrinking    // 縮小中（1→0）
};

class Player;

class PlayerEffect
{
public:

	void Init();

	void Update();

	void Draw();

	void SetPlayer(Player* player) { player_ = player; }

	// 移動時のエフェクトを呼び出す
	void OnceMoveEffect();
	// 弾のトレイルエフェクトを呼び出す
	void OnceBulletTrailEffect(const int32_t count, const WorldTransform& transform);
	// 弾が消える時のエフェクトを呼び出す
	void OnceBulletDeleteEffect(const int32_t count, const WorldTransform& transform);
	// 弾のエフェクトを呼び出す
	void OnceBulletEffect();
	// 避けた時のエフェクトを呼び出す
	void OnceAvoidEffect();
	// 必殺技を撃った時のエフェクト
	void UpdatePostEffect();
	bool GetIsGrayScale()const { return isGrayScale_; }
	void SetIsGrayScale(bool flag) { isGrayScale_ = flag; }

private:

	Player* player_ = nullptr;

	// パーティクルを管理する
	ParticleManager* particleManager_ = ParticleManager::GetInstance();

	/* ==================== プレイヤーが移動時のエフェクト ==================== */

	// 移動時の土埃
	std::unique_ptr<ParticleEmitter> moveDustEmitter_ = nullptr;

	/* ==================== プレイヤーが弾を撃った時のエフェクト ==================== */

	// 弾のトレイルエフェクト
	std::array<std::unique_ptr<ParticleEmitter>, 6> bulletTrailEmitters_;
	// 弾が消える時のエフェクト
	std::array<std::unique_ptr<ParticleEmitter>, 6> bulletDeleteEmitters_;

	/* ==================== プレイヤーが弾を撃った時のエフェクト ==================== */

	// 弾を撃った時の爆発
	std::unique_ptr<ParticleEmitter> bulletExplosionEmitter_ = nullptr;
	// 弾を撃った時の火花
	std::unique_ptr<ParticleEmitter> bulletSparkEmitter_ = nullptr;
	// 弾を撃った後の煙
	std::unique_ptr<ParticleEmitter> bulletSmokeEmitter_ = nullptr;
	// 弾を撃った後の薬莢
	std::unique_ptr<ParticleEmitter> bulletCartridgeEmitter_ = nullptr;

	/* ==================== プレイヤーが避けた時のエフェクト ==================== */

	// 避けた時の土埃
	std::unique_ptr<ParticleEmitter> avoidDustEmitter_ = nullptr;

	/* ==================== プレイヤーが必殺技を撃った時のエフェクト ==================== */

	GrayscaleState grayscaleState_ = GrayscaleState::None;
	bool isGrayScale_ = false;
	float grayscaleFrame_ = 0.0f;
};

