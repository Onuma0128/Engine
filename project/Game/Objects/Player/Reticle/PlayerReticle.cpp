#include "PlayerReticle.h"

#include "WinApp.h"
#include "Input.h"
#include "CameraManager.h"
#include "imgui.h"

#include "Camera.h"
#include "DeltaTimer.h"


void PlayerReticle::Init()
{
	Sprite::Initialize("2dReticle.png");

	transform_.size = { 0.0f,0.0f };
	anchorPoint_ = { 0.5f,0.5f };
	Sprite::GetRenderOptions().enabled = false;
	Sprite::GetRenderOptions().offscreen = false;
	Sprite::SetColor({ 1.0f,1.0f,1.0f,1.0f });
	Sprite::Update();

	Collider::AddCollider();
	Collider::myType_ = ColliderType::kSegment;
	Collider::colliderName_ = "PlayerReticle";
	Collider::isActive_ = false;
	Collider::targetColliderName_ = { "Enemy" };
	hitCount_ = 0;
	reticleColorTimer_ = 2.0f;
}

void PlayerReticle::Update(bool isPlayingMouse)
{
	Input* input = Input::GetInstance();

	// 移動の処理
	const float reticleSpeed = 10.0f;
	Vector2 velocity{};
	if (!isPlayingMouse) {
		velocity.x = input->GetGamepadRightStickX();
		velocity.y = -input->GetGamepadRightStickY();
	} else {
		transform_.position.x = static_cast<float>(input->GetMousePosX());
		transform_.position.y = static_cast<float>(input->GetMousePosY());
	}

	if (transform_.size.x >= 64.0f) {
		transform_.position += velocity * reticleSpeed;
	}

	// 当たり判定用の線を更新
	SegmentUpdate();

	// カラーをラープさせる
	if (reticleColorTimer_ <= 1.0f) {
		reticleColorTimer_ += 1.0f / 30.0f;
		float color = std::clamp(reticleColorTimer_, 0.0f, 1.0f);
		Sprite::SetColor(Vector4{ 1.0f,color,color,1.0f });

		if (reticleColorTimer_ <= 0.0f) {
			float time = reticleColorTimer_ * -5.0f;
			transform_.size = (Vector2::MochiPuniScaleNormalized(time) * 64.0f);
		}
	}

	// Spriteの更新
	Sprite::Update();
}

void PlayerReticle::Draw()
{
	if (Sprite::GetRenderOptions().enabled) {
		Sprite::Draw();
	}
}

void PlayerReticle::OnCollisionEnter(Collider* other)
{
	// 当たった敵のTransformを作成して取得する
	if (other->GetColliderName() == "Enemy" &&
		hitCount_ < 6) {
		reticleColorTimer_ = -0.2f;
		++hitCount_;
		WorldTransform transform;
		transform.translation_ = other->GetCenterPosition();
		enemyColliders_.push_front(other);
		Input::GetInstance()->Vibrate(0.2f, 1.0f, 10);
	}
}

void PlayerReticle::OnCollisionStay(Collider* other)
{
}

void PlayerReticle::OnCollisionExit(Collider* other)
{
}

void PlayerReticle::SegmentUpdate()
{
	if (hitCount_ >= 6) { Collider::isActive_ = false; }
	else { Collider::isActive_ = true; }

	Vector2 position = transform_.position;
	Vector3 ndc = {
		(position.x / static_cast<float>(WinApp::kClientWidth)) * 2.0f - 1.0f,
		-((position.y / static_cast<float>(WinApp::kClientHeight)) * 2.0f - 1.0f),
		1.0f
	};
	Matrix4x4 invVP = Matrix4x4::Inverse(CameraManager::GetInstance()->GetActiveCamera()->GetViewProjectionMatrix());
	Vector3 nearPos = Vector3::Transform(Vector3(ndc.x, ndc.y, 0.0f), invVP);
	Vector3 farPos = Vector3::Transform(Vector3(ndc.x, ndc.y, 1.0f), invVP);

	Collider::origin_ = nearPos;
	Collider::diff_ = (farPos - nearPos);
}

void PlayerRayReticle::Init()
{
	Sprite::Initialize("2dReticle.png");
	transform_.size = { 64.0f,64.0f };
	anchorPoint_ = { 0.5f,0.5f };
	Sprite::GetRenderOptions().enabled = true;
	Sprite::GetRenderOptions().offscreen = false;
	Sprite::SetColor({ 1.0f,1.0f,1.0f,1.0f });
	Sprite::Update();
}

void PlayerRayReticle::Update()
{
	Sprite::Update();
}

void PlayerRayReticle::Draw()
{
	Sprite::Draw();
}

void PlayerRayReticle::SetRaticleAlpha(bool flag)
{
	if (flag) { alphaTimer_ += DeltaTimer::GetDeltaTime() * 5.0f; }
	else { alphaTimer_ -= DeltaTimer::GetDeltaTime() * 5.0f; }
	alphaTimer_ = std::clamp(alphaTimer_, 0.0f, 1.0f);

	Vector4 color = { alphaTimer_,0.0f,0.0f,alphaTimer_ };
	Sprite::SetColor(color);
}

void PlayerRayReticle::SetPosition(const Vector3& position)
{
	// ワールドからスクリーン座標に変換
	if (position.Length() < 0.01f) { return; }
	Vector3 screenPos = Vector3::Transform(
		position,
		CameraManager::GetInstance()->GetActiveCamera()->GetViewProjectionMatrix()
	);
	Vector2 pos = {
		((screenPos.x + 1.0f) / 2.0f) * static_cast<float>(WinApp::kClientWidth),
		((1.0f - screenPos.y) / 2.0f) * static_cast<float>(WinApp::kClientHeight)
	};

	if(Vector2::Distance(transform_.position, pos) > 320.0f){
		transform_.position = pos;
	} else {
		transform_.position = Vector2::Lerp(transform_.position, pos, 0.5f);
	}
}