#pragma once

#include "Collider.h"
#include "PrimitiveDrawr.h"

/// <summary>
/// ボスのダッシュ攻撃予測エフェクトをスケール変化させる
/// </summary>
class BossDashPrediction : public NumaEngine::Collider
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
    void Update(NumaEngine::PrimitiveDrawr* plane, NumaEngine::PrimitiveDrawr* timePlane);

	/// <summary>
	/// セッター
	/// </summary>
	/// <param name="scale"></param>
    void SetBossPosition(const NumaEngine::Vector3& position) { bossPosition_ = position; }
	void SetBossRotate(const NumaEngine::Quaternion& rotate) { bossRotate_ = rotate; }

private:

	/// <summary>
	/// 当たり判定
	/// </summary>
	/// <param name="other"></当たったColliderのポインタが入る>
    void OnCollisionEnter(NumaEngine::Collider* other) override;
	void OnCollisionStay(NumaEngine::Collider* other) override;
	void OnCollisionExit(NumaEngine::Collider* other) override;

	// リセット
	void Reset();

public:

	// ボスの回転
    NumaEngine::Quaternion bossRotate_ = {};
	// ボスの座標
    NumaEngine::Vector3 bossPosition_ = {};

	float scaleX_ = 0.0f;

	float hitDistance_ = 100.0f;
	float newHitDistance_ = 0.0f;
    NumaEngine::Vector3 hitPosition_ = {};
	NumaEngine::Vector3 newHitPosition_ = {};
	bool isHit_ = false;
	float time_ = 0.0f;

};



