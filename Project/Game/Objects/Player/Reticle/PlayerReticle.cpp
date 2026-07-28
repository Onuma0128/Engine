#include "PlayerReticle.h"

#include <numbers>

#include "WinApp.h"
#include "Input.h"
#include "CameraManager.h"
#include "imgui.h"

#include "Camera.h"
#include "DeltaTimer.h"
#include "Easing.h"

void PlayerReticle::Init()
{
    this->NumaEngine::Sprite::Initialize("2dReticle.png");

	transform_.size = { 0.0f,0.0f };
	anchorPoint_ = { 0.5f,0.5f };
	GetRenderOptions().enabled = false;
	GetRenderOptions().offscreen = false;
    this->NumaEngine::Sprite::SetColor({ 1.0f,1.0f,1.0f,1.0f });
	this->NumaEngine::Sprite::Update();

    NumaEngine::Collider::AddCollider();
	NumaEngine::Collider::myType_ = NumaEngine::ColliderType::kSegment;
	NumaEngine::Collider::colliderName_ = "PlayerReticle";
	NumaEngine::Collider::isActive_ = false;
	NumaEngine::Collider::targetColliderName_ = { "Enemy" };
	hitCount_ = 0;
	reticleColorTimer_ = 2.0f;
}

void PlayerReticle::Update(bool isPlayingMouse)
{
	Input* input = Input::GetInstance();

	// 遘ｻ蜍輔・蜃ｦ逅・
    const float reticleSpeed = 10.0f;
	NumaEngine::Vector2 velocity{};
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

	// 蠖薙◆繧雁愛螳夂畑縺ｮ邱壹ｒ譖ｴ譁ｰ
	SegmentUpdate();

	// 繧ｫ繝ｩ繝ｼ繧偵Λ繝ｼ繝励＆縺帙ｋ
	if (reticleColorTimer_ <= 1.0f) {
		reticleColorTimer_ += 1.0f / 30.0f;
		float color = std::clamp(reticleColorTimer_, 0.0f, 1.0f);
        this->NumaEngine::Sprite::SetColor(NumaEngine::Vector4{ 1.0f,color,color,1.0f });

		if (reticleColorTimer_ <= 0.0f) {
			float time = reticleColorTimer_ * -5.0f;
			transform_.size = (NumaEngine::Vector2::MochiPuniScaleNormalized(time) * 64.0f);
		}
	}

	// Sprite縺ｮ譖ｴ譁ｰ
    this->NumaEngine::Sprite::Update();
}

void PlayerReticle::Draw()
{
    if (GetRenderOptions().enabled) {
		this->NumaEngine::Sprite::Draw();
	}
}

void PlayerReticle::OnCollisionEnter(NumaEngine::Collider* other)
{
	// 蠖薙◆縺｣縺滓雰縺ｮTransform繧剃ｽ懈・縺励※蜿門ｾ励☆繧・
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

void PlayerReticle::OnCollisionStay(NumaEngine::Collider* other)
{
}

void PlayerReticle::OnCollisionExit(NumaEngine::Collider* other)
{
}

void PlayerReticle::SegmentUpdate()
{
    if (hitCount_ >= 6) { NumaEngine::Collider::isActive_ = false; }
	else { NumaEngine::Collider::isActive_ = true; }

    NumaEngine::Vector2 position = transform_.position;
    NumaEngine::Vector3 ndc = {
		(position.x / static_cast<float>(NumaEngine::WinApp::kClientWidth)) * 2.0f - 1.0f,
		-((position.y / static_cast<float>(NumaEngine::WinApp::kClientHeight)) * 2.0f - 1.0f),
		1.0f
	};
    NumaEngine::Matrix4x4 invVP = NumaEngine::Matrix4x4::Inverse(CameraManager::GetInstance()->GetActiveCamera()->GetViewProjectionMatrix());
    NumaEngine::Vector3 nearPos = NumaEngine::Vector3::Transform(NumaEngine::Vector3(ndc.x, ndc.y, 0.0f), invVP);
	NumaEngine::Vector3 farPos = NumaEngine::Vector3::Transform(NumaEngine::Vector3(ndc.x, ndc.y, 1.0f), invVP);

    NumaEngine::Collider::origin_ = nearPos;
	NumaEngine::Collider::diff_ = (farPos - nearPos);
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
    if (flag) { alphaTimer_ += NumaEngine::DeltaTimer::GetDeltaTime() * 5.0f; }
	else { alphaTimer_ -= NumaEngine::DeltaTimer::GetDeltaTime() * 5.0f; }
	alphaTimer_ = std::clamp(alphaTimer_, 0.0f, 1.0f);
	float t = std::sinf(alphaTimer_ * std::numbers::pi_v<float>);

    transform_.size = NumaEngine::Vector2{ 64.0f,64.0f } + NumaEngine::Vector2{ 32.0f,32.0f } * t;
	transform_.rotate = std::numbers::pi_v<float> * NumaEngine::Easing::EaseInBack(alphaTimer_);
	transform_.rotate = std::numbers::pi_v<float> * NumaEngine::Easing::EaseInBack(alphaTimer_);

	NumaEngine::Vector4 color = { alphaTimer_,0.0f,0.0f,alphaTimer_ };
	Sprite::SetColor(color);
}

void PlayerRayReticle::SetPosition(const NumaEngine::Vector3& position)
{
	// 繝ｯ繝ｼ繝ｫ繝峨°繧峨せ繧ｯ繝ｪ繝ｼ繝ｳ蠎ｧ讓吶↓螟画鋤
	if (position.Length() < 0.01f) { return; }
	NumaEngine::Vector3 screenPos = NumaEngine::Vector3::Transform(
		position,
		CameraManager::GetInstance()->GetActiveCamera()->GetViewProjectionMatrix()
	);
        NumaEngine::Vector2 pos = {
		((screenPos.x + 1.0f) / 2.0f) * static_cast<float>(NumaEngine::WinApp::kClientWidth),
		((1.0f - screenPos.y) / 2.0f) * static_cast<float>(NumaEngine::WinApp::kClientHeight)
	};

    if(NumaEngine::Vector2::Distance(transform_.position, pos) > 320.0f){
		transform_.position = pos;
	} else {
        transform_.position = NumaEngine::Vector2::Lerp(transform_.position, pos, 0.5f);
	}
}


