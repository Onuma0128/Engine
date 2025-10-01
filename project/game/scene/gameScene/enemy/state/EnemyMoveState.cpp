#include "EnemyMoveState.h"

#include <memory>

#include "DeltaTimer.h"

#include "../base/BaseEnemy.h"

#include "gameScene/player/Player.h"
#include "gameScene/enemy/adjustItem/EnemyAdjustItem.h"

#include "gameScene/enemy/type/EnemyType.h"

#include "Melee/EnemyMelee_AttackState.h"
#include "Ranged/EnemyRanged_AttackState.h"
#include "ShieldBearer/EnemyShieldBearer_AttackState.h"
#include "RangedElite/EnemyRangedElite_AttackState.h"

EnemyMoveState::EnemyMoveState(BaseEnemy* enemy) :EnemyBaseState(enemy) {}

void EnemyMoveState::Init()
{
	attackCoolTime_ = GetTypeAttackCoolTime();

	isMoveAnima_ = false;
	isIdleAnima_ = false;

	enemy_->ResetSearch();
	enemy_->GetEnemyRay()->SetActive(true);
	enemy_->GetEnemyRay()->Reset();
}

void EnemyMoveState::Finalize()
{
	auto& pathFinder = enemy_->GetPathFinder();
	pathFinder.DebugSpline(false);

	enemy_->GetEnemyRay()->SetActive(false);
}

void EnemyMoveState::Update()
{
	// 探索をする
	searchTime_ += DeltaTimer::GetDeltaTime();
	if (searchTime_ > enemy_->GetItem()->GetMainData().searchUpdateTime) {
		searchTime_ = 0.0f;
		enemy_->ResetSearch();
	}

	// 方向を計算
	const float speed = GetTypeSpeed();
	auto& pathFinder = enemy_->GetPathFinder();

	// 探索の更新
	pathFinder.Update(speed);
	pathFinder.DebugSpline(enemy_->GetItem()->GetMainData().debugSpline);
	Vector3 velocity = pathFinder.GetVelocity();
	velocity.y = 0.0f;
	if (velocity.Length() != 0.0f) { velocity = velocity.Normalize(); }
	enemy_->SetVelocity(velocity);

	// 移動時の回転の処理
	if (velocity.Length() != 0.0f) {
		Quaternion yRotation = pathFinder.GetRotation();
		enemy_->GetTransform().rotation_ = Quaternion::Slerp(enemy_->GetTransform().rotation_, yRotation, 0.2f);
	}

	// 攻撃のクールタイムを縮める
	attackCoolTime_ -= DeltaTimer::GetDeltaTime();
	attackCoolTime_ = std::clamp(attackCoolTime_, 0.0f, 100.0f);

	// 攻撃のクールタイムが0になっているなら攻撃ステートに遷移
	Vector3 playerPos = enemy_->GetPlayer()->GetTransform().translation_;
	Vector3 enemyPos = enemy_->GetTransform().translation_ + (velocity * speed * DeltaTimer::GetDeltaTime());
	const float dist = Vector3::Distance(enemyPos, playerPos);
	// 入り判定、出判定
	const float attackIn = GetTypeAttackDistance();
	const float margin = enemy_->GetItem()->GetMainData().margin;
	const float attackOut = attackIn + margin;
	if (!inAttackRange_ && dist <= attackIn)  inAttackRange_ = true;
	if (inAttackRange_ && dist >= attackOut) inAttackRange_ = false;

	Vector3 direction = Vector3::ExprUnitZ.Transform(Quaternion::MakeRotateMatrix(enemy_->GetTransform().rotation_));
	enemy_->GetEnemyRay()->Update(enemyPos, direction * attackIn);

	if (inAttackRange_) {
		if (enemy_->GetEnemyRay()->GetLooking()) {
			// 待機時アニメーションにする
			AttackCoolTimeAnimation();
		} else {
			// 移動時アニメーションにする
			MoveAnimation();
			// 距離があれば移動処理をする
			enemy_->GetTransform().translation_ += velocity * speed * DeltaTimer::GetDeltaTime();
			enemy_->GetEnemyRay()->Reset();
		}
		if (attackCoolTime_ <= 0.0f && enemy_->GetEnemyRay()->GetLooking()) {
			TypeChengeAttackState();
			return;
		}
	} else {
		// 移動時アニメーションにする
		MoveAnimation();
		// 距離があれば移動処理をする
		enemy_->GetTransform().translation_ += velocity * speed * DeltaTimer::GetDeltaTime();
		enemy_->GetEnemyRay()->Reset();
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

const float EnemyMoveState::GetTypeAttackCoolTime()
{
	switch (enemy_->GetType()) {
	case EnemyType::Melee:			return enemy_->GetItem()->GetMeleeData().tempData.attackCoolTime;
	case EnemyType::Ranged:			return enemy_->GetItem()->GetRangedData().tempData.attackCoolTime;
	case EnemyType::ShieldBearer:	return enemy_->GetItem()->GetShieldBearerData().tempData.attackCoolTime;
	case EnemyType::RangedElite:	return enemy_->GetItem()->GetRangedEliteData().tempData.attackCoolTime;
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

void EnemyMoveState::MoveAnimation()
{
	if (isMoveAnima_) { return; }

	bool played = false;
	switch (enemy_->GetType()) {
	case EnemyType::Melee:          played = enemy_->PlayByName("Run_Arms"); break;
	case EnemyType::Ranged:         played = enemy_->PlayByName("Run");      break;
	case EnemyType::ShieldBearer:   played = enemy_->PlayByName("Run_Arms"); break;
	case EnemyType::RangedElite:    played = enemy_->PlayByName("Run");      break;
	}

	if (played) {
		isMoveAnima_ = true;
		isIdleAnima_ = false;
	}
}

void EnemyMoveState::AttackCoolTimeAnimation()
{
	if (isIdleAnima_) { return; }

	bool played = false;
	switch (enemy_->GetType()) {
	case EnemyType::Melee:          played = enemy_->PlayByName("Idle"); break;
	case EnemyType::Ranged:         played = enemy_->PlayByName("Idle"); break;
	case EnemyType::ShieldBearer:   played = enemy_->PlayByName("Idle"); break;
	case EnemyType::RangedElite:    played = enemy_->PlayByName("Idle"); break;
	}

	if (played) {
		isIdleAnima_ = true;
		isMoveAnima_ = false;
	}
}
