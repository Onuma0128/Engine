#include "PlayerReticle.h"

#include "WinApp.h"
#include "Input.h"
#include "CameraManager.h"

#include "Camera.h"
#include "DeltaTimer.h"


void PlayerReticle::Init()
{
	Sprite::Initialize("2dReticle.png");

	transform_.size = { 0.0f,0.0f };
	anchorPoint_ = { 0.5f,0.5f };
	GetRenderOptions().enabled = false;
	GetRenderOptions().offscreen = false;
	Sprite::Update();

	Collider::AddCollider();
	Collider::myType_ = ColliderType::Segment;
	Collider::colliderName_ = "2dReticle";
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

	// 当たり判定用の線を更新
	SegmentUpdate();

	// Spriteの更新
	Sprite::Update();
}

void PlayerReticle::OnCollisionStay(Collider* other)
{
	if (other->GetColliderName() == "Enemy") {
		SetColor({ 1.0f,0.0f,0.0f,1.0f });
	}
}

void PlayerReticle::OnCollisionExit(Collider* other)
{
	if (other->GetColliderName() == "Enemy") {
		SetColor({ 1.0f,1.0f,1.0f,1.0f });
	}
}

void PlayerReticle::SegmentUpdate()
{
	Vector2 position = transform_.position;
	Vector3 ndc = {
		(position.x / WinApp::kClientWidth) * 2.0f - 1.0f,
		-((position.y / WinApp::kClientHeight) * 2.0f - 1.0f),
		1.0f
	};
	Matrix4x4 invVP = Matrix4x4::Inverse(CameraManager::GetInstance()->GetActiveCamera()->GetViewProjectionMatrix());
	Vector3 nearPos = Vector3::Transform(Vector3(ndc.x, ndc.y, 0.0f), invVP);
	Vector3 farPos = Vector3::Transform(Vector3(ndc.x, ndc.y, 1.0f), invVP);

	Collider::origin_ = nearPos;
	Collider::diff_ = farPos - nearPos;
}
