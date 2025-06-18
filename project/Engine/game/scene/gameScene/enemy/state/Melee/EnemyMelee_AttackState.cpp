#include "EnemyMelee_AttackState.h"

#include <memory>

#include "DeltaTimer.h"

#include "gameScene/enemy/enemy.h"
#include "gameScene/enemy/adjustItem/EnemyAdjustItem.h"
#include "../EnemyMoveState.h"

EnemyMelee_AttackState::EnemyMelee_AttackState(Enemy* enemy) :EnemyBaseState(enemy) {}

void EnemyMelee_AttackState::Init()
{
	chengeStateTime_ = 0.0f;
	enemy_->SetColor(Vector4{ 0.0f,0.0f,1.0f,1.0f });

	Collider::AddCollider();
	Collider::myType_ = ColliderType::Sphere;
	Collider::colliderName_ = "EnemyMelee";
	Collider::DrawCollider();
}

void EnemyMelee_AttackState::Finalize()
{
	enemy_->SetColor(Vector4{ 1.0f,0.0f,0.0f,1.0f });
	Collider::RemoveCollider();
}

void EnemyMelee_AttackState::Update()
{
	// 近接攻撃のデータを取得
	MeleeData data = enemy_->GetItem()->GetMeleeData();

	chengeStateTime_ += DeltaTimer::GetDeltaTime();

	// 攻撃を始める硬直時間
	float time = data.tempData.attackStartupTime;
	Collider::isActive_ = false;
	if (time < chengeStateTime_) {
		// 攻撃を行っている時間
		time += data.tempData.attackActiveTime;
		Collider::isActive_ = true;
		if (time < chengeStateTime_) {
			// 攻撃が終わってからの硬直時間
			time += data.tempData.attackRecoveryTime;
			Collider::isActive_ = false;
			if (time < chengeStateTime_) {
				enemy_->ChengeState(std::make_unique<EnemyMoveState>(enemy_));
				return;
			}
		}
	}

	// コライダーの更新
	Collider::radius_ = data.colliderSize;
	Vector3 colliderOffset = data.colliderOffset.Transform(Quaternion::MakeRotateMatrix(enemy_->GetTransform().rotation_));
	Collider::centerPosition_ = enemy_->GetTransform().translation_ + colliderOffset;
	Collider::Update();
}

void EnemyMelee_AttackState::Draw()
{
}

void EnemyMelee_AttackState::OnCollisionEnter(Collider* other)
{
}

void EnemyMelee_AttackState::OnCollisionStay(Collider* other)
{
}

void EnemyMelee_AttackState::OnCollisionExit(Collider* other)
{
}
