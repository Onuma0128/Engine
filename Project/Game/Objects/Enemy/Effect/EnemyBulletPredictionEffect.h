#pragma once
#include <memory>

#include "Collider.h"
#include "PrimitiveDrawr.h"

/// <summary>
/// 敵の弾の攻撃予測エフェクト
/// </summary>
class EnemyBulletPredictionEffect : public Collider
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
	/// セッター
	/// </summary>
	/// <param name="scale"></param>
	void SetScale(const Vector3& scale) { plane_->GetTransform().scale = scale; }
	void SetRotate(const Quaternion& rotate) { plane_->GetTransform().rotation = rotate; }
	void SetTranslate(const Vector3& translate) { plane_->GetTransform().translation = translate; }
	void SetAddUvPosition(const Vector2& position) { plane_->GetUVTransform().position += position; }
	void SetEnabledDraw(bool flag) { plane_->GetRenderOptions().enabled = flag; }
	void SetEnemyPosition(const Vector3& position) { enemyPosition_ = position; }

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

private:

	// エフェクト
	std::unique_ptr<PrimitiveDrawr> plane_ = nullptr;
	float hitDistance_ = 100.0f;
	Vector3 hitPosition_ = {};
	bool isHit_ = false;

	// 敵の座標
	Vector3 enemyPosition_ = {};

};

