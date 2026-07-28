#include "BossRay.h"

#include "Collision3D.h"

#include "Collision/CollisionFilter.h"

void BossRay::Init()
{
	// コライダーを設定
    this->AddCollider();
	this->myType_ = NumaEngine::ColliderType::kSegment;
	this->colliderName_ = "BossRay";
	this->isActive_ = false;
	this->targetColliderName_ = {
		"Player","Building","DeadTree","fence","Bush","StoneWall","ShortStoneWall",
	};
    this->DrawCollider();

	isLooking_ = false;
}

void BossRay::Update(const NumaEngine::Vector3& start, const NumaEngine::Vector3& end)
{
	// 毎フレーム初期化する
	hitPointLength_ = 1000.0f;

	// 座標を保存
	start_ = start;
	end_ = end;

    // 反射処理のコライダーを設定
	origin_ = start_;
	diff_ = end_;
	this->LineUpdate();
}

void BossRay::SetActive(const bool flag)
{
    this->isActive_ = flag;
	this->LineUpdate();
}

void BossRay::Reset()
{
	isLooking_ = false;
}

void BossRay::OnCollisionEnter(NumaEngine::Collider* other)
{
}

void BossRay::OnCollisionStay(NumaEngine::Collider* other)
{

	const auto& name = other->GetColliderName();
	const auto type = other->GetMyColliderType();

	RaycastHit hit{};
	if (CollisionFilter::CheckColliderNameFieldObject(other->GetColliderName())) {
        if (type == NumaEngine::ColliderType::kOBB) {
			if (NumaEngine::Collision3D::OBBSegment(other, this, &hit)) {
				float length = (hit.point - start_).Length();
				if (hitPointLength_ < length) { return; }
				hitPointLength_ = length;
				isLooking_ = false;
			}
		} else {
            if (NumaEngine::Collision3D::SphereSegment(other, this, &hit)) {
				float length = (hit.point - start_).Length();
				if (hitPointLength_ < length) { return; }
				hitPointLength_ = length;
				isLooking_ = false;
			}
		}
	}
    if (name == "Player") {
		if (NumaEngine::Collision3D::SphereSegment(other, this, &hit)) {
			float length = (hit.point - start_).Length();
			if (hitPointLength_ < length) { return; }
			hitPointLength_ = length;
			isLooking_ = true;
		}
	}
}

void BossRay::OnCollisionExit(NumaEngine::Collider* other)
{
	const auto& name = other->GetColliderName();

	if (name == "Player") {
		isLooking_ = false;
	}
}

