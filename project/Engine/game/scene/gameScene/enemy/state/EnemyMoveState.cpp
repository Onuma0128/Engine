#include "EnemyMoveState.h"

#include <memory>

#include "DeltaTimer.h"

#include "gameScene/enemy/enemy.h"
#include "gameScene/player/Player.h"
#include "gameScene/enemy/adjustItem/EnemyAdjustItem.h"

#include "gameScene/enemy/type/EnemyType.h"

#include "Melee/EnemyMelee_AttackState.h"
#include "Ranged/EnemyRanged_AttackState.h"
#include "ShieldBearer/EnemyShieldBearer_AttackState.h"
#include "RangedElite/EnemyRangedElite_AttackState.h"

EnemyMoveState::EnemyMoveState(Enemy* enemy) :EnemyBaseState(enemy) {}

void EnemyMoveState::Init()
{
}

void EnemyMoveState::Finalize()
{
}

void EnemyMoveState::Update()
{
	// 移動処理
	const float speed = GetTypeSpeed();
	Vector3 velocity = 
		Vector3(enemy_->GetPlayer()->GetTransform().translation_ - enemy_->GetTransform().translation_);
	velocity.y = 0.0f;
	if (velocity.Length() != 0.0f) { velocity = velocity.Normalize(); }
	enemy_->GetTransform().translation_ += velocity * speed * DeltaTimer::GetDeltaTime();
	enemy_->SetVelocity(velocity);

	// 移動時の回転の処理
	if (velocity.Length() != 0.0f) {
		Vector3 foward = Vector3::ExprUnitZ;
		Vector3 targetDir = Vector3{ -velocity.x,0.0f,velocity.z };

		// velocityから回転を求める
		Matrix4x4 targetMatrix = Matrix4x4::DirectionToDirection(foward, targetDir);
		Quaternion targetRotation = Quaternion::FormRotationMatrix(targetMatrix);
		Quaternion currentRotation = enemy_->GetTransform().rotation_;
		Quaternion result = Quaternion::Slerp(currentRotation, targetRotation, 0.1f);

		// 回転を適応
		enemy_->GetTransform().rotation_ = result;
	}

	// 攻撃ステートに遷移
	if (Vector3::Distance(enemy_->GetTransform().translation_, enemy_->GetPlayer()->GetTransform().translation_) <=
		enemy_->GetItem()->GetMeleeData().tempData.attackDistance) {
		TypeChengeAttackState();
	}
}

void EnemyMoveState::Draw()
{
}

const float EnemyMoveState::GetTypeSpeed()
{
	switch (enemy_->GetType()) {
	case EnemyType::Melee:			return enemy_->GetItem()->GetMeleeData().tempData.speed;
	case EnemyType::Ranged:			return enemy_->GetItem()->GetRangedData().tempData.speed;
	case EnemyType::ShieldBearer:	return enemy_->GetItem()->GetShieldBearerData().tempData.speed;
	case EnemyType::RangedElite:	return enemy_->GetItem()->GetRangedEliteData().tempData.speed;
	default:break;
	}
	return 0.0f;
}

const float EnemyMoveState::GetTypeAttackDistance()
{
	switch (enemy_->GetType()) {
	case EnemyType::Melee:			return enemy_->GetItem()->GetMeleeData().tempData.attackDistance;
	case EnemyType::Ranged:			return enemy_->GetItem()->GetRangedData().tempData.attackDistance;
	case EnemyType::ShieldBearer:	return enemy_->GetItem()->GetShieldBearerData().tempData.attackDistance;
	case EnemyType::RangedElite:	return enemy_->GetItem()->GetRangedEliteData().tempData.attackDistance;
	default:break;
	}
	return 0.0f;
}

void EnemyMoveState::TypeChengeAttackState()
{
	switch (enemy_->GetType()) {
	case EnemyType::Melee:			enemy_->ChengeState(std::make_unique<EnemyMelee_AttackState>(enemy_)); break;
	case EnemyType::Ranged:			enemy_->ChengeState(std::make_unique<EnemyRanged_AttackState>(enemy_)); break;
	case EnemyType::ShieldBearer:	enemy_->ChengeState(std::make_unique<EnemyShieldBearer_AttackState>(enemy_)); break;
	case EnemyType::RangedElite:	enemy_->ChengeState(std::make_unique<EnemyRangedElite_AttackState>(enemy_)); break;
	default:break;
	}
}
