#include "EnemyShield.h"

#include "gameScene/enemy/Enemy.h"
#include "gameScene/enemy/adjustItem/EnemyAdjustItem.h"

EnemyShield::EnemyShield(Enemy* enemy) : EnemyWeaponBase(enemy) {}

void EnemyShield::Init(ColliderType type, const std::string& name)
{
	Collider::AddCollider();
	Collider::myType_ = type;
	Collider::colliderName_ = name;
	Collider::isActive_ = false;
	Collider::DrawCollider();
}

void EnemyShield::Update()
{
	Collider::Update();
}

void EnemyShield::OnCollisionEnter(Collider* other)
{
}

void EnemyShield::OnCollisionStay(Collider* other)
{
}

void EnemyShield::OnCollisionExit(Collider* other)
{
}

void EnemyShield::SetIsActive(bool flag)
{
	Collider::isActive_ = flag;
}
