#include "EnemyShield.h"

#include "DeltaTimer.h"

#include "gameScene/player/Player.h"
#include "gameScene/enemy/Enemy.h"
#include "gameScene/enemy/adjustItem/EnemyAdjustItem.h"

EnemyShield::EnemyShield(Enemy* enemy) : EnemyWeaponBase(enemy) {}

void EnemyShield::Finalize()
{
	Object3d::RemoveRenderer();
	Collider::RemoveCollider();
}

void EnemyShield::Init(ColliderType type, const std::string& name)
{
	Object3d::Initialize("Shield_Heater.obj");
	Object3d::SetSceneRenderer();

	Collider::AddCollider();
	Collider::myType_ = type;
	Collider::colliderName_ = name;
	Collider::DrawCollider();

	rotateY_ = Quaternion::IdentityQuaternion();
	rotateMatrix_ = Matrix4x4::Identity();
}

void EnemyShield::Update()
{
	ShieldBearerData data = enemy_->GetItem()->GetShieldBearerData();

	Vector3 direction = (enemy_->GetPlayer()->GetTransform().translation_ - enemy_->GetTransform().translation_);
	direction.y = 0.0f;
	if (direction.Length() != 0.0f) {
		direction = direction.Normalize();
		rotateY_ = Quaternion::DirectionToQuaternion(rotateY_, direction, data.lerpSpeed * DeltaTimer::GetDeltaTime());
		rotateMatrix_ = Quaternion::MakeRotateMatrix(rotateY_);
	}
	Collider::size_ = data.shieldColliderSize;
	Collider::rotate_ = rotateY_;
	Collider::centerPosition_ = enemy_->GetTransform().translation_ + data.shieldColliderOffset.Transform(rotateMatrix_);
	Collider::Update();

	transform_.scale_ = data.shieldSize;
	transform_.rotation_ = rotateY_;
	transform_.translation_ = enemy_->GetTransform().translation_ + data.shieldOffset.Transform(rotateMatrix_);
	Object3d::Update();
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
	Object3d::GetMaterial().enableDraw = flag;
	Collider::isActive_ = flag;
}
