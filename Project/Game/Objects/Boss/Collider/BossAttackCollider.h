#pragma once

#include "Collider.h"
#include "Vector3.h"

// 前方宣言
class BossEnemy;

/// <summary>
/// 敵の攻撃用コライダー
/// </summary>
class BossAttackCollider : public NumaEngine::Collider
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	// セッター
    void SetBossEnemy(BossEnemy* boss) { boss_ = boss; }
	void SetColliderSize(float size) { colliderSize_ = size; }
	void SetColliderOffset(const NumaEngine::Vector3& offset) { colliderOffset_ = offset; }
	void SetActive(bool flag) { this->isActive_ = flag; }
	void SetRadius(float radius) { this->radius_ = radius; }

private:

	/// <summary>
	/// 当たり判定
	/// </summary>
	/// <param name="other"></当たったColliderのポインタが入る>
    void OnCollisionEnter(NumaEngine::Collider* other) override;
	void OnCollisionStay(NumaEngine::Collider* other) override;
	void OnCollisionExit(NumaEngine::Collider* other) override;

private:

	// ボス本体のポインタ
	BossEnemy* boss_ = nullptr;

	// コライダーのサイズと座標
	float colliderSize_ = 0.0f;
    NumaEngine::Vector3 colliderOffset_ = {};

};




