#include "EnemyBulletPredictionEffect.h"

#include "Collision3D.h"

void EnemyBulletPredictionEffect::Init()
{
	// コライダーを設定
	Collider::AddCollider();
	Collider::myType_ = ColliderType::kSegment;
	Collider::colliderName_ = "EnemyBulletRay";
	Collider::isActive_ = false;
	Collider::targetColliderName_ = {
		"Building","DeadTree","fence","Bush","StoneWall","ShortStoneWall","MuscleCompanion",
	};
	Collider::DrawCollider();

	// エフェクトの初期化
	plane_ = std::make_unique<PrimitiveDrawr>();
	plane_->TypeInit(PrimitiveType::kPlane);
	plane_->SetBlendMode(BlendMode::kBlendModeAdd);
	plane_->SetColor(Vector3::ExprUnitX);
	plane_->SetAlpha(0.5f);
	plane_->SetSceneRenderer();
	plane_->GetRenderOptions().enabled = false;
}

void EnemyBulletPredictionEffect::Update()
{
	// コライダーを設定する
	Collider::origin_ = enemyPosition_ + Vector3::ExprUnitY;
	Collider::diff_ = (plane_->GetTransform().translation - enemyPosition_) * 2.0f;
	Collider::isActive_ = plane_->GetRenderOptions().enabled;

	if (isHit_) {
		plane_->GetTransform().scale.y = hitDistance_ * 0.5f;
		float y = plane_->GetTransform().translation.y;
		plane_->GetTransform().translation = (enemyPosition_ + hitPosition_) * 0.5f;
		plane_->GetTransform().translation.y = y;
	}
	// コライダーとエフェクトの更新
	Collider::Update();
	plane_->Update();
	this->Reset();
}

void EnemyBulletPredictionEffect::OnCollisionEnter(Collider* other)
{
}

void EnemyBulletPredictionEffect::OnCollisionStay(Collider* other)
{
	const auto& name = other->GetColliderName();
	const auto type = other->GetMyColliderType();
	RaycastHit hit{};

	if (type == ColliderType::kOBB) {
		if (Collision3D::OBBSegment(other, this, &hit)) {
			float distance = Vector3::Distance(hit.point, enemyPosition_);
			if (hitDistance_ < distance) { return; }
			hitDistance_ = distance;
			hitPosition_ = hit.point;
			isHit_ = true;
		}
	} else {
		if (Collision3D::SphereSegment(other, this, &hit)) {
			float distance = Vector3::Distance(hit.point, enemyPosition_);
			if (hitDistance_ < distance) { return; }
			hitDistance_ = distance;
			hitPosition_ = hit.point;
			isHit_ = true;
		}
	}
	hitPosition_.y = 0.0f;
}

void EnemyBulletPredictionEffect::OnCollisionExit(Collider* other)
{
}

void EnemyBulletPredictionEffect::Reset()
{
	hitDistance_ = 100.0f;
	isHit_ = false;
}