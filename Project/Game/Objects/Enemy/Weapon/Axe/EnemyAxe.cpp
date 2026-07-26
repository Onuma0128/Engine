#include "EnemyAxe.h"

#include "objects/enemy/base/BaseEnemy.h"
#include "objects/enemy/adjustItem/EnemyAdjustItem.h"

EnemyAxe::EnemyAxe(BaseEnemy* enemy) : EnemyWeaponBase(enemy) {}

void EnemyAxe::Finalize()
{
	Collider::RemoveCollider();
}

void EnemyAxe::Init(NumaEngine::ColliderType type, const std::string& name)
{
    NumaEngine::Collider::AddCollider();
	NumaEngine::Collider::myType_ = type;
	NumaEngine::Collider::colliderName_ = name;
	NumaEngine::Collider::isActive_ = false;
	NumaEngine::Collider::targetColliderName_ = {
		"Player","MuscleCompanion"
	};
	NumaEngine::Collider::DrawCollider();
}

void EnemyAxe::Update()
{
    if ("EnemyMelee" == NumaEngine::Collider::colliderName_) {
		MeleeData data = enemy_->GetItem()->GetMeleeData();

        NumaEngine::Collider::radius_ = data.colliderSize;
        NumaEngine::Matrix4x4 rotateMatrix = NumaEngine::Quaternion::MakeRotateMatrix(enemy_->GetTransform().rotation_);
        NumaEngine::Collider::centerPosition_ = enemy_->GetTransform().translation_ + data.colliderOffset.Transform(rotateMatrix);

	} else {
		ShieldBearerData data = enemy_->GetItem()->GetShieldBearerData();

        NumaEngine::Collider::radius_ = data.attackColliderSize;
        NumaEngine::Matrix4x4 rotateMatrix = NumaEngine::Quaternion::MakeRotateMatrix(enemy_->GetTransform().rotation_);
        NumaEngine::Collider::centerPosition_ = enemy_->GetTransform().translation_ + data.attackColliderOffset.Transform(rotateMatrix);
	}

	Collider::Update();
}

void EnemyAxe::OnCollisionEnter(NumaEngine::Collider* other)
{
}

void EnemyAxe::OnCollisionStay(NumaEngine::Collider* other)
{
}

void EnemyAxe::OnCollisionExit(NumaEngine::Collider* other)
{
}

void EnemyAxe::SetIsActive(bool flag)
{
	NumaEngine::Collider::isActive_ = flag;
}

bool EnemyAxe::GetIsActive()
{
	return NumaEngine::Collider::isActive_;
}

