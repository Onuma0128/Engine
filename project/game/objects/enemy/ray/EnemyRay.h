#pragma once

#include "Collider.h"

class EnemyRay : public Collider
{
public:

	void Init();

	void Update(const Vector3& start, const Vector3& end);

	void Finalize();

	bool GetLooking()const { return isLooking_; }

	void SetActive(const bool flag);

	void Reset();

private:

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

