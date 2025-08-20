#include "PlayerDeadState.h"

#include "gameScene/player/Player.h"

#include "gameScene/player/state/PlayerMoveState.h"

PlayerDeadState::PlayerDeadState(Player* player) :PlayerBaseState(player) {}

void PlayerDeadState::Init()
{
	player_->GetEffect()->OnceHitEffect();
}

void PlayerDeadState::Finalize()
{
}

void PlayerDeadState::Update()
{
	player_->ChengeState(std::make_unique<PlayerMoveState>(player_));
}

void PlayerDeadState::Draw()
{
}
