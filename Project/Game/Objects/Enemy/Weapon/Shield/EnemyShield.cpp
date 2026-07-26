#include "EnemyShield.h"

#include "DeltaTimer.h"

#include "objects/enemy/base/BaseEnemy.h"
#include "objects/enemy/adjustItem/EnemyAdjustItem.h"
#include "objects/player/Player.h"

EnemyShield::EnemyShield(BaseEnemy* enemy) : EnemyWeaponBase(enemy) {}

void EnemyShield::Finalize()
{
	Object3d::RemoveRenderer();
	NumaEngine::Collider::RemoveCollider();
}

void EnemyShield::Init(NumaEngine::ColliderType type, const std::string& name)
{
	Object3d::Initialize("Shield_Heater.obj");
	Object3d::SetSceneRenderer();
	Object3d::GetMaterial().outlineMask = true;
	Object3d::GetMaterial().outlineSceneColor = true;
	Object3d::GetMaterial().outlineColor = NumaEngine::Vector3::ExprZero;

    NumaEngine::Collider::AddCollider();
	NumaEngine::Collider::myType_ = type;
	NumaEngine::Collider::colliderName_ = name;
	NumaEngine::Collider::targetColliderName_ = {
		"MuscleCompanion","SearchDashMuscleCompanion",
	};
	NumaEngine::Collider::DrawCollider();

	rotateY_ = NumaEngine::Quaternion::IdentityQuaternion();
    rotateMatrix_ = NumaEngine::Matrix4x4::Identity();
}

void EnemyShield::Update()
{
	ShieldBearerData data = enemy_->GetItem()->GetShieldBearerData();

	NumaEngine::Vector3 direction = (enemy_->GetPlayer()->GetTransform().translation_ - enemy_->GetTransform().translation_);
	direction.y = 0.0f;
	if (direction.Length() != 0.0f) {
		direction = direction.Normalize();
		rotateY_ = NumaEngine::Quaternion::DirectionToQuaternion(rotateY_, direction, data.lerpSpeed * DeltaTimer::GetDeltaTime());
		rotateMatrix_ = NumaEngine::Quaternion::MakeRotateMatrix(rotateY_);
	}
    NumaEngine::Collider::size_ = data.shieldColliderSize;
	NumaEngine::Collider::rotate_ = rotateY_;
	NumaEngine::Collider::centerPosition_ = enemy_->GetTransform().translation_ + data.shieldColliderOffset.Transform(rotateMatrix_);
	NumaEngine::Collider::Update();

	transform_.scale_ = data.shieldSize;
	transform_.rotation_ = rotateY_;
	transform_.translation_ = enemy_->GetTransform().translation_ + data.shieldOffset.Transform(rotateMatrix_);
	Object3d::Update();
}

void EnemyShield::OnCollisionEnter(NumaEngine::Collider* other)
{
}

void EnemyShield::OnCollisionStay(NumaEngine::Collider* other)
{
}

void EnemyShield::OnCollisionExit(NumaEngine::Collider* other)
{
}

void EnemyShield::SetIsActive(bool flag)
{
	Object3d::GetMaterial().enableDraw = flag;
    NumaEngine::Collider::isActive_ = flag;
}

bool EnemyShield::GetIsActive()
{
    return NumaEngine::Collider::isActive_;
}

