#pragma once

#include "Collider.h"

// 前方宣言
class MuscleCompanion;

/// <summary>
/// 後続判定用コライダークラス
/// </summary>
class CompanionFollowerCollider : public NumaEngine::Collider
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
	void SetActive(bool flag) { NumaEngine::Collider::isActive_ = flag; }
	void SetRadius(float radius) { NumaEngine::Collider::radius_ = radius; }

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
};

