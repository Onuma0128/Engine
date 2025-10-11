#include "EnemyRay.h"

#include "Collision3D.h"

void EnemyRay::Init()
{
	// コライダーを設定
	Collider::AddCollider();
	Collider::myType_ = ColliderType::Segment;
	Collider::colliderName_ = "EnemyRay";
	Collider::isActive_ = false;
	Collider::targetColliderName_ = {
		"Player","Building","DeadTree","fence","Bush","StoneWall","ShortStoneWall",
	};
	Collider::DrawCollider();

	isLooking_ = false;
}

void EnemyRay::Update(const Vector3& start, const Vector3& end)
{
	// 毎フレーム初期化する
	hitPointLength_ = 1000.0f;

	// 座標を保存
	start_ = start + Vector3{ 0.0f,0.5f,0.0f };
	end_ = end;

	// 反射処理のコライダーを設定
	Collider::origin_ = start_;
	Collider::diff_ = end_;
	Collider::LineUpdate();
}

void EnemyRay::Finalize()
{
	Collider::RemoveCollider();
}

void EnemyRay::SetActive(const bool flag)
{
	Collider::isActive_ = flag;
	Collider::LineUpdate();
}

void EnemyRay::Reset()
{
	isLooking_ = false;
}

void EnemyRay::OnCollisionEnter(Collider* other)
{
}

void EnemyRay::OnCollisionStay(Collider* other)
{

	const auto& name = other->GetColliderName();
	const auto type = other->GetMyColliderType();

	RaycastHit hit{};
	if (name == "Building" || name == "DeadTree" || name == "fence" ||
		name == "Bush" || name == "StoneWall" || name == "ShortStoneWall") {
		if (type == ColliderType::OBB) {
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

void EnemyRay::OnCollisionExit(Collider* other)
{
	const auto& name = other->GetColliderName();

	if (name == "Player") {
		isLooking_ = false;
	}
}