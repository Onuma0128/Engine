#pragma once

#include "Collider.h"

/// <summary>
/// ボスの視線クラス
/// </summary>
class BossRay : public Collider
{
public:

	/// <summary>
	///	初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="start"></視線のスタート>
	/// <param name="end"></視線の終わり>
	void Update(const Vector3& start, const Vector3& end);

	/// <summary>
	/// 見えているかのゲッター
	/// </summary>
	/// <returns></returns>
	bool GetLooking()const { return isLooking_; }

	/// <summary>
	///	視線の有効無効を切り替える
	/// </summary>
	/// <param name="flag"></param>
	void SetActive(const bool flag);

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

private:

	/// <summary>
	/// コライダーに当たった時
	/// </summary>
	/// <param name="other"></当たったColliderのポインタが入る>
	void OnCollisionEnter(Collider* other) override;
	void OnCollisionStay(Collider* other) override;
	void OnCollisionExit(Collider* other) override;

private:

	// ヒットした壁が近いかを判別する距離
	float hitPointLength_ = 1000.0f;
	// 更新で受け取ったラインの座標
	Vector3 start_{};
	Vector3 end_{};

	// 見えているか
	bool isLooking_ = false;

};

