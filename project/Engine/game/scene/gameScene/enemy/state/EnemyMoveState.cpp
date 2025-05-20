#include "EnemyMoveState.h"

#include "DeltaTimer.h"

#include "gameScene/enemy/enemy.h"
#include "gameScene/player/Player.h"

EnemyMoveState::EnemyMoveState(Enemy* enemy) :EnemyBaseState(enemy) {}

void EnemyMoveState::Init()
{
}

void EnemyMoveState::Finalize()
{
}

void EnemyMoveState::Update()
{
	const float speed = 1.0f;
	Vector3 velocity = Vector3(
		enemy_->GetPlayer()->GetTransform().translation_ - 
		enemy_->GetTransform().translation_).Normalize();
	enemy_->GetTransform().translation_ += velocity * speed * DeltaTimer::GetDeltaTime();
}

void EnemyMoveState::Draw()
{
}
