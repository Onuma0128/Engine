#pragma once

#include "Collider.h"

// 前方宣言
class MuscleCompanion;

/// <summary>
/// 後続判定用コライダークラス
/// </summary>
class CompanionFollowerCollider : public Collider
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
	void SetActive(bool flag) { Collider::isActive_ = flag; }
	void SetRadius(float radius) { Collider::radius_ = radius; }

private:

	/// <summary>
	/// 当たり判定
	/// </summary>
	/// <param name="other"></当たったColliderのポインタが入る>
	void OnCollisionEnter(Collider* other) override;
	void OnCollisionStay(Collider* other) override;
	void OnCollisionExit(Collider* other) override;

private:

	// 仲間本体のポインタ
	MuscleCompanion* companion_ = nullptr;
};

