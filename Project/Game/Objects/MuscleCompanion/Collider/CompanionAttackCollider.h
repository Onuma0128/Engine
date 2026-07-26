#pragma once

#include <vector>

#include "Collider.h"

// 前方宣言
class MuscleCompanion;

/// <summary>
/// 仲間の攻撃用コライダークラス
/// </summary>
class CompanionAttackCollider : public NumaEngine::Collider
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
	void SetCompanion(MuscleCompanion* companion) { companion_ = companion; }
	void SetActive(bool flag) { this->isActive_ = flag; }
	void SetRadius(float radius) { this->radius_ = radius; }
	void ResetHitColliders() { hitColliders_.clear(); }

	// ゲッター
    const std::vector<NumaEngine::Collider*>& GetHitColliders() { return hitColliders_; }
	const bool GetIsHit() { return isHit_; }

private:

	/// <summary>
	/// 当たり判定
	/// </summary>
	/// <param name="other"></当たったColliderのポインタが入る>
    void OnCollisionEnter(NumaEngine::Collider* other) override;
	void OnCollisionStay(NumaEngine::Collider* other) override;
	void OnCollisionExit(NumaEngine::Collider* other) override;

private:

	// 仲間本体のポインタ
	MuscleCompanion* companion_ = nullptr;

	// 当たったかどうかのフラグ
    std::vector<NumaEngine::Collider*> hitColliders_;
	bool isHit_ = true;

};

