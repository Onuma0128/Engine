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

private:

	// パーティクルを管理する
	ParticleManager* particleManager_ = ParticleManager::GetInstance();

	/* ==================== 弾がヒット時のエフェクト ==================== */

	// 移動時の土埃
	std::shared_ptr<ParticleEmitter> hitEmitter_ = nullptr;
};

