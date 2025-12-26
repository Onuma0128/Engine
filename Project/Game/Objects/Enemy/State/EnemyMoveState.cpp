#include "EnemyMoveState.h"

#include <memory>

#include "DeltaTimer.h"

#include "objects/enemy/base/BaseEnemy.h"

#include "objects/player/Player.h"

#include "Objects/Enemy/AdjustItem/EnemyAdjustItem.h"
#include "Objects/enemy/type/EnemyType.h"
#include "Objects/enemy/ray/EnemyRay.h"

EnemyMoveState::EnemyMoveState(BaseEnemy* enemy) :EnemyBaseState(enemy) {}

void EnemyMoveState::Init()
{
	attackCoolTime_ = GetTypeAttackCoolTime();

	isMoveAnima_ = false;
	isIdleAnima_ = false;

	enemy_->ResetSearch();
	//enemy_->GetEnemyRay()->SetActive(true);
	enemy_->GetEnemyRay()->Reset();
}

void EnemyMoveState::Finalize()
{
	auto& pathFinder = enemy_->GetPathFinder();
	pathFinder.DebugSpline(false);

	//enemy_->GetEnemyRay()->SetActive(false);
}

void EnemyMoveState::Update()
{
	// データを取得
	const auto mainData = enemy_->GetItem()->GetMainData();

	// 探索をする
	searchTime_ += DeltaTimer::GetDeltaTime();
	if (searchTime_ > mainData.searchUpdateTime) {
		searchTime_ = 0.0f;
		enemy_->ResetSearch();
	}

	// 方向を計算
	const float speed = GetTypeSpeed();
	auto& pathFinder = enemy_->GetPathFinder();

	// 探索の更新
	pathFinder.Update(speed);
	pathFinder.DebugSpline(mainData.debugSpline);
	Vector3 velocity = pathFinder.GetVelocity();
	velocity.y = 0.0f;
	if (velocity.Length() != 0.0f) { velocity = velocity.Normalize(); }
	enemy_->SetVelocity(velocity);

	// 移動時の回転の処理
	if (velocity.Length() != 0.0f) {
		Quaternion yRotation = pathFinder.GetRotation();
		enemy_->SetTransformRotation(Quaternion::Slerp(enemy_->GetTransform().rotation_, yRotation, 0.2f));
	}

	// 攻撃のクールタイムを縮める
	attackCoolTime_ -= DeltaTimer::GetDeltaTime();
	attackCoolTime_ = std::clamp(attackCoolTime_, 0.0f, 100.0f);

	// 攻撃のクールタイムが0になっているなら攻撃ステートに遷移
	Vector3 playerPos = enemy_->GetPlayer()->GetTransform().translation_;
	if (enemy_->GetHitCollider()) { playerPos = enemy_->GetHitCollider()->GetCenterPosition(); }
	Vector3 enemyPos = enemy_->GetTransform().translation_ + (velocity * speed * DeltaTimer::GetDeltaTime());
	const float dist = Vector3::Distance(enemyPos, playerPos);
	// 入り判定、出判定
	const float attackIn = enemy_->GetTypeAttackDistance();
	const float margin = mainData.margin;
	const float attackOut = attackIn + margin;
	if (!inAttackRange_ && dist <= attackIn)  inAttackRange_ = true;
	if (inAttackRange_ && dist >= attackOut) inAttackRange_ = false;

	if (inAttackRange_) {
		if (enemy_->GetEnemyRay()->GetLooking()) {
			// 待機時アニメーションにする
			AttackCoolTimeAnimation();
		} else {
			// 移動時のアクションを行う
			MoveAction(velocity, speed);
		}
		if (attackCoolTime_ <= 0.0f && enemy_->GetEnemyRay()->GetLooking()) {
			enemy_->TypeChengeAttackState();
			return;
		}
	} else {
		// 移動時のアクションを行う
		MoveAction(velocity, speed);
	}
}

void EnemyMoveState::Draw()
{
}

const float EnemyMoveState::GetTypeSpeed()
{
	switch (enemy_->GetType()) {
	case EnemyType::kMelee:			return enemy_->GetItem()->GetMeleeData().tempData.speed;
	case EnemyType::kRanged:		return enemy_->GetItem()->GetRangedData().tempData.speed;
	case EnemyType::kShieldBearer:	return enemy_->GetItem()->GetShieldBearerData().tempData.speed;
	case EnemyType::kRangedElite:	return enemy_->GetItem()->GetRangedEliteData().tempData.speed;
	default:break;
	}
	return 0.0f;
}

const float EnemyMoveState::GetTypeAttackCoolTime()
{
	switch (enemy_->GetType()) {
	case EnemyType::kMelee:			return enemy_->GetItem()->GetMeleeData().tempData.attackCoolTime;
	case EnemyType::kRanged:		return enemy_->GetItem()->GetRangedData().tempData.attackCoolTime;
	case EnemyType::kShieldBearer:	return enemy_->GetItem()->GetShieldBearerData().tempData.attackCoolTime;
	case EnemyType::kRangedElite:	return enemy_->GetItem()->GetRangedEliteData().tempData.attackCoolTime;
	default:break;
	}
	return 0.0f;
}

void EnemyMoveState::MoveAction(const Vector3& velocity, const float speed)
{
	// 移動時アニメーションにする
	MoveAnimation();
	enemy_->GetEffect()->OnceMoveEffect(enemy_->GetTransform());
	// 距離があれば移動処理をする
	Vector3 position = enemy_->GetTransform().translation_;
	enemy_->SetTransformTranslation(position + velocity * speed * DeltaTimer::GetDeltaTime());
	enemy_->GetEnemyRay()->Reset();
}

void EnemyMoveState::MoveAnimation()
{
	if (isMoveAnima_) { return; }

	bool played = false;
	switch (enemy_->GetType()) {
	case EnemyType::kMelee:          played = enemy_->PlayByName("Run_Arms"); break;
	case EnemyType::kRanged:         played = enemy_->PlayByName("Run");      break;
	case EnemyType::kShieldBearer:   played = enemy_->PlayByName("Run_Arms"); break;
	case EnemyType::kRangedElite:    played = enemy_->PlayByName("Run");      break;
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
	case EnemyType::kMelee:          played = enemy_->PlayByName("Idle"); break;
	case EnemyType::kRanged:         played = enemy_->PlayByName("Idle"); break;
	case EnemyType::kShieldBearer:   played = enemy_->PlayByName("Idle"); break;
	case EnemyType::kRangedElite:    played = enemy_->PlayByName("Idle"); break;
	}

	if (played) {
		isIdleAnima_ = true;
		isMoveAnima_ = false;
	}
}
