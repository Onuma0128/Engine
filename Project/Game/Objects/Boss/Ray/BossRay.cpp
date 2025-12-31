#include "BossRay.h"

#include "Collision3D.h"

#include "Collision/CollisionFilter.h"

void BossRay::Init()
{
	// コライダーを設定
	Collider::AddCollider();
	Collider::myType_ = ColliderType::kSegment;
	Collider::colliderName_ = "BossRay";
	Collider::isActive_ = false;
	Collider::targetColliderName_ = {
		"Player","Building","DeadTree","fence","Bush","StoneWall","ShortStoneWall",
	};
	Collider::DrawCollider();

	isLooking_ = false;
}

void BossRay::Update(const Vector3& start, const Vector3& end)
{
	// 毎フレーム初期化する
	hitPointLength_ = 1000.0f;

	// 座標を保存
	start_ = start;
	end_ = end;

	// 反射処理のコライダーを設定
	Collider::origin_ = start_;
	Collider::diff_ = end_;
	Collider::LineUpdate();
}

void BossRay::SetActive(const bool flag)
{
	Collider::isActive_ = flag;
	Collider::LineUpdate();
}

void BossRay::Reset()
{
	isLooking_ = false;
}

void BossRay::OnCollisionEnter(Collider* other)
{
}

void BossRay::OnCollisionStay(Collider* other)
{

	const auto& name = other->GetColliderName();
	const auto type = other->GetMyColliderType();

	RaycastHit hit{};
	if (CollisionFilter::CheckColliderNameFieldObject(other->GetColliderName())) {
		if (type == ColliderType::kOBB) {
			if (Collision3D::OBBSegment(other, this, &hit)) {
				float length = (hit.point - start_).Length();
				if (hitPointLength_ < length) { return; }
				hitPointLength_ = length;
				isLooking_ = false;
			}
		} else {
			if (Collision3D::SphereSegment(other, this, &hit)) {
				float length = (hit.point - start_).Length();
				if (hitPointLength_ < length) { return; }
				hitPointLength_ = length;
				isLooking_ = false;
			}
		}
	}
	if (name == "Player") {
		if (Collision3D::SphereSegment(other, this, &hit)) {
			float length = (hit.point - start_).Length();
			if (hitPointLength_ < length) { return; }
			hitPointLength_ = length;
			isLooking_ = true;
		}
	}
}

void BossRay::OnCollisionExit(Collider* other)
{
	const auto& name = other->GetColliderName();

	if (name == "Player") {
		isLooking_ = false;
	}
}