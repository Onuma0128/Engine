#include "PlayerAvoidState.h"

#include <numbers>

#include "Input.h"
#include "DeltaTimer.h"

#include "objects/player/Player.h"
#include "objects/player/adjustItem/PlayerAdjustItem.h"
#include "PlayerMoveState.h"

PlayerAvoidState::PlayerAvoidState(Player* player) :PlayerBaseState(player) {}

void PlayerAvoidState::Init()
{
	player_->SetIsAvoid(true);

	velocity_ = CreateVelocity();
	PlayerData data = player_->GetItem()->GetPlayerData();
	velocityY_ = data.avoid_velocityY;
	acceleration_ = data.avoid_acceleration;

	rotateY_ = Quaternion::ExtractYawQuaternion(player_->GetTransform().rotation_);
	if (velocity_.Length() == 0.0f) {
		Matrix4x4 rotateMatrix = Quaternion::MakeRotateMatrix(rotateY_);
		velocity_ = Vector3::ExprUnitZ.Transform(rotateMatrix).Normalize();
	} else {
		player_->SetTransformRotation(
			Quaternion::DirectionToQuaternion(player_->GetTransform().rotation_, velocity_, 1.0f));
		rotateY_ = Quaternion::ExtractYawQuaternion(player_->GetTransform().rotation_);
	}

	player_->SetAvoidCoolTimer(data.avoid_coolTime);
}

void PlayerAvoidState::Finalize()
{
}

void PlayerAvoidState::Update()
{
	PlayerData data = player_->GetItem()->GetPlayerData();

	float speed = data.avoid_speed;
	if (player_->GetIsPushMove()) { speed = data.speed; }
	avoidTime_ += DeltaTimer::GetDeltaTime() * (1.0f / data.avoidTime);
	avoidTime_ = std::clamp(avoidTime_, 0.0f, 1.0f);

	// 回転を適応
	Quaternion rotateX = Quaternion::MakeRotateAxisAngleQuaternion
	(Vector3::ExprUnitX, (avoidTime_ * 2.0f) * std::numbers::pi_v<float>);
	player_->SetTransformRotation(rotateY_ * rotateX);
	// 座標を更新
	Vector3 position = player_->GetTransform().translation_;
	player_->SetTransformTranslation(position + velocity_ * speed * DeltaTimer::GetDeltaTime());
	velocityY_ -= acceleration_ * DeltaTimer::GetDeltaTime();
	Vector2 min = player_->GetItem()->GetPlayerData().minPlayerClamp;
	Vector2 max = player_->GetItem()->GetPlayerData().maxPlayerClamp;
	position = player_->GetTransform().translation_;
	position.x = std::clamp(position.x, min.x, max.x);
	position.y += velocityY_;
	position.z = std::clamp(position.z, min.y, max.y);
	player_->SetTransformTranslation(position);
	// 回避終了判定
	if (avoidTime_ >= 1.0f) {
		player_->GetTimeStop() = false;
		player_->SetIsAvoid(false);
		position.y = 0.0f;
		player_->SetTransformTranslation(position);
		player_->SetTransformRotation(player_->GetShot()->GetRightStickQua());
		player_->ChengeState(std::make_unique<PlayerMoveState>(player_));
	} else {
		if (!chengeAnimation_) {
			if (player_->PlayByName("Duck", 0.0f)) {
				player_->GetTimeStop() = true;
				chengeAnimation_ = true;
			}
		}
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
