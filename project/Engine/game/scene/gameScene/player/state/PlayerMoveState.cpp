#include "PlayerMoveState.h"

#include "Input.h"
#include "DeltaTimer.h"

#include "gameScene/player/Player.h"
#include "PlayerAvoidState.h"
#include "PlayerSpecialMoveState.h"

PlayerMoveState::PlayerMoveState(Player* player) :PlayerBaseState(player) {}

void PlayerMoveState::Init()
{
}

void PlayerMoveState::Finalize()
{
}

void PlayerMoveState::Update()
{
	// Inputを取得
	Input* input = Input::GetInstance();

	// 移動の処理
	const float playerSpeed = 3.0f;
	Vector3 velocity{};
	velocity.x = input->GetGamepadLeftStickX();
	velocity.z = input->GetGamepadLeftStickY();

	player_->GetTransform().translation_ += velocity * DeltaTimer::GetDeltaTime() * playerSpeed;

	// 移動時の回転の処理
	if (velocity.Length() != 0.0f) {
		Vector3 foward = Vector3::ExprUnitZ;
		Vector3 targetDir = Vector3{ -velocity.x,0.0f,velocity.z };

		// velocityから回転を求める
		Matrix4x4 targetMatrix = Matrix4x4::DirectionToDirection(foward, targetDir);
		Quaternion targetRotation = Quaternion::FormRotationMatrix(targetMatrix);
		Quaternion currentRotation = player_->GetTransform().rotation_;
		Quaternion result = Quaternion::Slerp(currentRotation, targetRotation, 0.1f);

		// 回転を適応
		player_->GetTransform().rotation_ = result;

		player_->GetEffect()->OnceMoveEffect();
	}

	// 弾を発射する(弾を発射するとリロードが止まる)
	if (input->TriggerGamepadButton(XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
		isReloadBullet_ = false;
		reloadBulletTime_ = 0.0f;
		player_->AttackBullet();
	}
	// 弾のリロードを開始する
	if (input->TriggerGamepadButton(XINPUT_GAMEPAD_LEFT_SHOULDER)) {
		isReloadBullet_ = true;
	}
	// リロードをする関数
	ReloadBullet();

	// 避けの状態に遷移
	if (input->TriggerGamepadButton(XINPUT_GAMEPAD_B)) {
		player_->GetEffect()->OnceAvoidEffect();
		player_->ChengeState(std::make_unique<PlayerAvoidState>(player_));
		return;
	}

	if (input->GetGamepadRightTrigger() != 0.0f && !player_->GetEffect()->GetIsGrayScale()) {
		player_->GetEffect()->SetIsGrayScale(true);
	}
}

void PlayerMoveState::Draw()
{
}

void PlayerMoveState::ReloadBullet()
{
	if (isReloadBullet_) {
		// 前の時間を取得
		int32_t previousTime = static_cast<int32_t>(reloadBulletTime_);
		// リロードした瞬間一発リロードする
		if (reloadBulletTime_ == 0.0f) {
			player_->ReloadBullet();
		}

		// 今の時間を更新
		reloadBulletTime_ += DeltaTimer::GetDeltaTime() * 10.0f;
		int32_t currentTime = static_cast<int32_t>(reloadBulletTime_);

		// 1秒立ったらリロードをする
		if (previousTime != currentTime) {
			player_->ReloadBullet();
		}
	}
}
