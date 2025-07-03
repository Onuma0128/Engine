#include "PlayerAvoidState.h"

#include <numbers>

#include "Input.h"
#include "DeltaTimer.h"

#include "gameScene/player/Player.h"
#include "PlayerMoveState.h"

PlayerAvoidState::PlayerAvoidState(Player* player) :PlayerBaseState(player) {}

void PlayerAvoidState::Init()
{
	player_->SetIsAvoid(true);

	velocity_ = CreateVelocity();
	velocityY_ = 16.0f;
	acceleration_ = 2.0f;

	rotateY_ = Quaternion::ExtractYawQuaternion(player_->GetTransform().rotation_);
	if (velocity_.Length() == 0.0f) {
		Matrix4x4 rotateMatrix = Quaternion::MakeRotateMatrix(rotateY_);
		velocity_ = Vector3::ExprUnitZ.Transform(rotateMatrix).Normalize();
	}
}

void PlayerAvoidState::Finalize()
{
}

void PlayerAvoidState::Update()
{
	const float speed = 20.0f;
	avoidTime_ += DeltaTimer::GetDeltaTime() * 4.0f;
	avoidTime_ = std::clamp(avoidTime_, 0.0f, 1.0f);

	// 回転を適応
	Quaternion rotateX = Quaternion::MakeRotateAxisAngleQuaternion
	(Vector3::ExprUnitX, (avoidTime_ * 2.0f) * std::numbers::pi_v<float>);
	player_->GetTransform().rotation_ = rotateY_ * rotateX;
	// 座標を更新
	player_->GetTransform().translation_ += velocity_ * speed * DeltaTimer::GetDeltaTime();

	velocityY_ -= acceleration_;
	player_->GetTransform().translation_.y += velocityY_ * DeltaTimer::GetDeltaTime();

	if (avoidTime_ >= 1.0f) {
		player_->SetIsAvoid(false);
		player_->GetTransform().translation_.y = 0.5f;
		player_->ChengeState(std::make_unique<PlayerMoveState>(player_));
	}
}

void PlayerAvoidState::Draw()
{
}

const Vector3 PlayerAvoidState::CreateVelocity()
{
	// Inputを取得
	Input* input = Input::GetInstance();
	Vector3 velocity{};

	velocity.x = input->GetGamepadLeftStickX();
	velocity.z = input->GetGamepadLeftStickY();
	// 左のスティックの入力が無ければ
	if (velocity.Length() == 0.0f) {
		if (input->PushKey(DIK_W)) {
			velocity.z = 1.0f;
		}
		if (input->PushKey(DIK_A)) {
			velocity.x = -1.0f;
		}
		if (input->PushKey(DIK_S)) {
			velocity.z = -1.0f;
		}
		if (input->PushKey(DIK_D)) {
			velocity.x = 1.0f;
		}
	}

	if (velocity.Length() != 0.0f) { return velocity.Normalize(); }
	return velocity;
}
