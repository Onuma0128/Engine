#include "EnemyAxe.h"

#include "gameScene/enemy/base/BaseEnemy.h"
#include "gameScene/enemy/adjustItem/EnemyAdjustItem.h"

EnemyAxe::EnemyAxe(BaseEnemy* enemy) : EnemyWeaponBase(enemy) {}

void EnemyAxe::Finalize()
{
	Collider::RemoveCollider();
}

void EnemyAxe::Init(ColliderType type, const std::string& name)
{
	Collider::AddCollider();
	Collider::myType_ = type;
	Collider::colliderName_ = name;
	Collider::isActive_ = false;
	Collider::DrawCollider();
}

void EnemyAxe::Update()
{
	if ("EnemyMelee" == Collider::colliderName_) {
		MeleeData data = enemy_->GetItem()->GetMeleeData();

		Collider::radius_ = data.colliderSize;
		Matrix4x4 rotateMatrix = Quaternion::MakeRotateMatrix(enemy_->GetTransform().rotation_);
		Collider::centerPosition_ = enemy_->GetTransform().translation_ + data.colliderOffset.Transform(rotateMatrix);

	} else {
		ShieldBearerData data = enemy_->GetItem()->GetShieldBearerData();

		Collider::radius_ = data.attackColliderSize;
		Matrix4x4 rotateMatrix = Quaternion::MakeRotateMatrix(enemy_->GetTransform().rotation_);
		Collider::centerPosition_ = enemy_->GetTransform().translation_ + data.attackColliderOffset.Transform(rotateMatrix);
	}

	Collider::Update();
}

void EnemyAxe::OnCollisionEnter(Collider* other)
{
}

void EnemyAxe::OnCollisionStay(Collider* other)
{
}

void EnemyAxe::OnCollisionExit(Collider* other)
{
}

void EnemyAxe::SetIsActive(bool flag)
{
	Collider::isActive_ = flag;
}
