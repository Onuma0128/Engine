#include "PlayerDeadState.h"

#include "objects/player/Player.h"

#include "PlayerMoveState.h"

PlayerDeadState::PlayerDeadState(Player* player) :PlayerBaseState(player) {}

void PlayerDeadState::Init()
{
	player_->GetEffect()->OnceHitEffect();
	player_->ForcePlayByName("Death", 0.1f);
	player_->SetAnimationTime(0.0f);
	player_->GetTimeStop() = true;
}

void PlayerDeadState::Finalize()
{
}

void PlayerDeadState::Update()
{
}

void PlayerDeadState::Draw()
{
}
