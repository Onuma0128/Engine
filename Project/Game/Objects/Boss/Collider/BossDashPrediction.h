#pragma once

#include "Collider.h"
#include "PrimitiveDrawr.h"

/// <summary>
/// ボスのダッシュ攻撃予測エフェクトをスケール変化させる
/// </summary>
class BossDashPrediction : public Collider
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	void Update(PrimitiveDrawr* plane, PrimitiveDrawr* timePlane);

	/// <summary>
	/// セッター
	/// </summary>
	/// <param name="scale"></param>
	void SetBossPosition(const Vector3& position) { bossPosition_ = position; }
	void SetBossRotate(const Quaternion& rotate) { bossRotate_ = rotate; }

private:

	/// <summary>
	/// 当たり判定
	/// </summary>
	/// <param name="other"></当たったColliderのポインタが入る>
	void OnCollisionEnter(Collider* other) override;
	void OnCollisionStay(Collider* other) override;
	void OnCollisionExit(Collider* other) override;

	// リセット
	void Reset();

public:

	// ボスの回転
	Quaternion bossRotate_ = {};
	// ボスの座標
	Vector3 bossPosition_ = {};

	float scaleX_ = 0.0f;

	float hitDistance_ = 100.0f;
	float newHitDistance_ = 0.0f;
	Vector3 hitPosition_ = {};
	Vector3 newHitPosition_ = {};
	bool isHit_ = false;
	float time_ = 0.0f;

};

