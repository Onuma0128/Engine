#include "BossDashPrediction.h"

#include "Collision3D.h"

void BossDashPrediction::Init()
{
	// コライダーを設定
	Collider::AddCollider();
	Collider::myType_ = ColliderType::kSegment;
	Collider::colliderName_ = "EnemyBulletRay";
	Collider::isActive_ = false;
	Collider::targetColliderName_ = {
		"Building","DeadTree","fence","Bush","StoneWall","ShortStoneWall",
	};
	Collider::DrawCollider();
}

void BossDashPrediction::Update(PrimitiveDrawr* plane, PrimitiveDrawr* timePlane)
{
	// コライダーを設定する
	Collider::origin_ = bossPosition_ + Vector3::ExprUnitY;
	Collider::diff_ = (plane->GetTransform().translation - bossPosition_) * 2.0f;
	Collider::isActive_ = plane->GetRenderOptions().enabled;

	if (isHit_) {
		plane->GetTransform().scale.y = hitDistance_ * 0.5f;
		timePlane->GetTransform().scale.y = hitDistance_ * 0.5f;
		float y = plane->GetTransform().translation.y;
		plane->GetTransform().translation = (bossPosition_ + hitPosition_) * 0.5f;
		plane->GetTransform().translation.y = y;
		timePlane->GetTransform().translation = plane->GetTransform().translation;
	}

	// コライダーの更新
	Collider::Update();
	plane->Update();
	this->Reset();
}

void BossDashPrediction::OnCollisionEnter(Collider* other)
{
}

void BossDashPrediction::OnCollisionStay(Collider* other)
{
	const auto& name = other->GetColliderName();
	const auto type = other->GetMyColliderType();
	RaycastHit hit{};

	if (type == ColliderType::kOBB) {
		if (Collision3D::OBBSegment(other, this, &hit)) {
			float distance = Vector3::Distance(hit.point, bossPosition_);
			if (hitDistance_ < distance) { return; }
			hitDistance_ = distance;
			hitPosition_ = hit.point;
			isHit_ = true;
		}
	} else {
		if (Collision3D::SphereSegment(other, this, &hit)) {
			float distance = Vector3::Distance(hit.point, bossPosition_);
			if (hitDistance_ < distance) { return; }
			hitDistance_ = distance;
			hitPosition_ = hit.point;
			isHit_ = true;
		}
	}
	hitPosition_.y = 0.0f;
}

void BossDashPrediction::OnCollisionExit(Collider* other)
{
}

void BossDashPrediction::Reset()
{
	hitDistance_ = 100.0f;
	isHit_ = false;
}