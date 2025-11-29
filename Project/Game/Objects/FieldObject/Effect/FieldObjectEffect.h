#pragma once
#include <string>

#include "ParticleManager.h"
#include "ParticleEmitter.h"

#include "WorldTransform.h"

/// <summary>
/// フィールドオブジェクトのエフェクトクラス
/// </summary>
class FieldObjectEffect
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="name"></フィールドオブジェクトの名前を取得する>
	void Init(const std::string& name);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 弾がヒット時のエフェクトを呼び出す
	/// </summary>
	/// <param name="transform"></フィールドオブジェクトのTransform>
	void OnceHitEffect(const WorldTransform& transform);

	/// <summary>
	/// ブレイク時のエフェクトを呼び出す
	/// </summary>
	/// <param name="transform"></フィールドオブジェクトのTransform>
	void OnceBreakEffect(const WorldTransform& transform);

private:

	// パーティクルを管理する
	ParticleManager* particleManager_ = ParticleManager::GetInstance();

	/* ==================== 弾がヒット時のエフェクト ==================== */

	std::shared_ptr<ParticleEmitter> hitEmitter_ = nullptr;

	/* ==================== 壊れた時のエフェクト ==================== */

	std::shared_ptr<ParticleEmitter> breakEmitter_ = nullptr;
	bool isBreakEffectPlay_ = false;
};

