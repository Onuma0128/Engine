#include "PlayerReticle.h"

#include "Input.h"
#include "DeltaTimer.h"

void PlayerReticle::Init()
{
	Sprite::Initialize("white1x1.png");

	transform_.size = { 0.0f,0.0f };
	anchorPoint_ = { 0.5f,0.5f };
	GetRenderOptions().enabled = false;
	GetRenderOptions().offscreen = false;
	Sprite::Update();
}

void PlayerReticle::GlobalInit()
{
}

void PlayerReticle::Update()
{
	Input* input = Input::GetInstance();

	// 移動の処理
	const float reticleSpeed = 500.0f;
	Vector2 velocity{};
	velocity.x = input->GetGamepadRightStickX();
	velocity.y = -input->GetGamepadRightStickY();

	transform_.position += velocity * DeltaTimer::GetDeltaTime() * reticleSpeed;

	Sprite::Update();
}
