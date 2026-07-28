#include "EnemyBulletPredictionEffect.h"

#include "Collision3D.h"

void EnemyBulletPredictionEffect::Init()
{
	// コライダーを設定
    this->AddCollider();
	this->myType_ = NumaEngine::ColliderType::kSegment;
	this->colliderName_ = "EnemyBulletRay";
	this->isActive_ = true;
	this->targetColliderName_ = {
		"Building","DeadTree","fence","Bush","StoneWall","ShortStoneWall","MuscleCompanion",
	};
    this->DrawCollider();

	// エフェクトの初期化
	plane_ = std::make_unique<PrimitiveDrawr>();
	plane_->TypeInit(PrimitiveType::kPlane);
	plane_->SetBlendMode(BlendMode::kBlendModeAdd);
	plane_->SetColor(NumaEngine::Vector3::ExprUnitX);
	plane_->SetAlpha(0.5f);
	plane_->SetSceneRenderer();
	plane_->GetRenderOptions().enabled = false;
}

void EnemyBulletPredictionEffect::Update()
{
    // コライダーを設定する
	this->origin_ = enemyPosition_ + NumaEngine::Vector3::ExprUnitY;
	this->diff_ = (plane_->GetTransform().translation - enemyPosition_) * 2.0f;

	if (isHit_) {
		plane_->GetTransform().scale.y = hitDistance_ * 0.5f;
		float y = plane_->GetTransform().translation.y;
		plane_->GetTransform().translation = (enemyPosition_ + hitPosition_) * 0.5f;
		plane_->GetTransform().translation.y = y;
	}
    // コライダーとエフェクトの更新
	NumaEngine::Collider::Update();
	plane_->Update();
	this->Reset();
}

void EnemyBulletPredictionEffect::OnCollisionEnter(NumaEngine::Collider* other)
{
}

void EnemyBulletPredictionEffect::OnCollisionStay(NumaEngine::Collider* other)
{
	const auto& name = other->GetColliderName();
	const auto type = other->GetMyColliderType();
	RaycastHit hit{};

    if (type == NumaEngine::ColliderType::kOBB) {
		if (NumaEngine::Collision3D::OBBSegment(other, this, &hit)) {
			float distance = NumaEngine::Vector3::Distance(hit.point, enemyPosition_);
			if (hitDistance_ < distance) { return; }
			hitDistance_ = distance;
			hitPosition_ = hit.point;
			isHit_ = true;
		}
    } else {
		if (NumaEngine::Collision3D::SphereSegment(other, this, &hit)) {
            float distance = NumaEngine::Vector3::Distance(hit.point, enemyPosition_);
			if (hitDistance_ < distance) { return; }
			hitDistance_ = distance;
			hitPosition_ = hit.point;
			isHit_ = true;
		}
	}
	hitPosition_.y = 0.0f;
}

void EnemyBulletPredictionEffect::OnCollisionExit(NumaEngine::Collider* other)
{
}

void EnemyBulletPredictionEffect::Reset()
{
	hitDistance_ = 100.0f;
	isHit_ = false;
}
