#pragma once

#include "PlayerBaseState.h"

class PlayerDeadState : public PlayerBaseState
{
public:

	PlayerDeadState(Player* player);

	void Init()override;

	void Finalize()override;

	void Update()override;

	void Draw()override;

private:

};

