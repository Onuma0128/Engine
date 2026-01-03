#include "PlayerEffect.h"

#include "DeltaTimer.h"
#include "Input.h"

#include "Easing.h"
#include "objects/player/Player.h"

PlayerEffect::~PlayerEffect()
{
	PostEffectManager::GetInstance()->GetGrayscaleData()->t = 0.0f;
	PostEffectManager::GetInstance()->GetVignetteData()->gamma = 0.0f;
}

void PlayerEffect::Init()
{
	// 移動時のエフェクト
	moveDustEmitter_ = std::make_unique<ParticleEmitter>("moveDust");
	particleManager_->CreateParticleGroup(moveDustEmitter_);
	moveDustEmitter_->SetIsCreate(false);

	// 避け時のエフェクト
	avoidDustEmitter_ = std::make_unique<ParticleEmitter>("avoidDust");
	particleManager_->CreateParticleGroup(avoidDustEmitter_);
	avoidDustEmitter_->SetIsCreate(false);

	// 攻撃を受けた時のエフェクト
	playerHit_ = std::make_unique<ParticleEmitter>("playerHit");
	particleManager_->CreateParticleGroup(playerHit_);
	playerHit_->SetIsCreate(false);

	playerReload_ = std::make_unique<PrimitiveDrawr>();
	playerReload_->TypeInit(PrimitiveType::kPlane);
	playerReload_->SetTexture("reloadUI.png");
	playerReload_->SetBlendMode(BlendMode::kBlendModeNone);
	playerReload_->GetRenderOptions().enabled = true;
	playerReload_->GetRenderOptions().offscreen = false;
	playerReload_->SetIsBillboard(true);
	playerReload_->GetTransform().scale = { 0.35f,0.35f,1.0f };

	// PostEffectを初期化
	postEffectManager_->CreatePostEffect(PostEffectType::kGrayscale);
	postEffectManager_->CreatePostEffect(PostEffectType::kVignette);
	postEffectManager_->CreatePostEffect(PostEffectType::kOutLine);

	needMoreMachoEffect = std::make_unique<PrimitiveDrawr>();
	needMoreMachoEffect->TypeInit(PrimitiveType::kPlane);
	needMoreMachoEffect->SetTexture("needMoreMacho.png");
	needMoreMachoEffect->SetBlendMode(BlendMode::kBlendModeNormal);
	needMoreMachoEffect->GetRenderOptions().enabled = true;
	needMoreMachoEffect->GetRenderOptions().offscreen = false;
	needMoreMachoEffect->SetIsBillboard(true);
	
	cylinder_ = std::make_unique<PrimitiveDrawr>();
	cylinder_->TypeInit(PrimitiveType::kCylinder, 32);
	cylinder_->GetTransform().scale = {};
	cylinder_->SetColor({ 1.0f,1.0f,0.0f });
	cylinder_->GetRenderOptions().enabled = false;
	cylinder_->GetRenderOptions().offscreen = false;
	cylinder_->SetBlendMode(BlendMode::kBlendModeAdd);
}

void PlayerEffect::Update()
{	
	UpdatePostEffect();

	cylinder_->GetTransform().translation = player_->GetTransform().translation_;
	cylinder_->GetTransform().translation.y = 0.0f;
	cylinder_->Update();

	playerReload_->GetTransform().translation = player_->GetTransform().translation_ + (Vector3::ExprUnitX * 0.6f);
	playerReload_->GetTransform().translation.y = 1.6f;
	playerReload_->GetTransform().rotation *= Quaternion::MakeRotateAxisAngleQuaternion(Vector3::ExprUnitZ, DeltaTimer::GetDeltaTime() * 9.0f);
	playerReload_->Update();

	specialMoveReadyTimer_ += DeltaTimer::GetDeltaTime();
	if (specialMoveReadyTimer_ > 1.0f) { specialMoveReadyTimer_ = 0.0f; }
	needMoreMachoEffect->GetTransform().translation = player_->GetTransform().translation_;
	needMoreMachoEffect->GetTransform().translation.y = 2.0f;
	needMoreMachoEffect->SetAlpha(std::abs(std::sin(specialMoveReadyTimer_ * std::numbers::pi_v<float>)));
	needMoreMachoEffect->Update();
}

void PlayerEffect::Draw()
{
	if (cylinder_->GetRenderOptions().enabled) {
		cylinder_->TypeDraw();
	}
	if (playerReload_->GetRenderOptions().enabled) {
		playerReload_->TypeDraw();
	}
	if (!player_->GetShot()->GetIsCanAttack()) {
		//needMoreMachoEffect->TypeDraw();
	}
}

void PlayerEffect::OnceMoveEffect()
{
	moveDustEmitter_->onceEmit();

	// パーティクルの座標を設定
	Quaternion rotate = player_->GetTransform().rotation_;
	Vector3 position = player_->GetTransform().translation_;
	
	moveDustEmitter_->SetRotation(rotate);
	moveDustEmitter_->SetPosition(position);
}

void PlayerEffect::OnceAvoidEffect()
{
	avoidDustEmitter_->onceEmit();

	// パーティクルの座標を設定
	Quaternion rotate = player_->GetTransform().rotation_;
	Vector3 position = player_->GetTransform().translation_;

	avoidDustEmitter_->SetPosition(position);
	avoidDustEmitter_->SetRotation(rotate);
}

void PlayerEffect::OnceHitEffect()
{
	playerHit_->onceEmit();

	// パーティクルの座標を設定
	Quaternion rotate = player_->GetTransform().rotation_;
	Vector3 position = player_->GetTransform().translation_;

	playerHit_->SetPosition(position);
	//playerHit_->SetRotation(rotate);
}

void PlayerEffect::UpdatePostEffect()
{
	const float expandDuration = 1.0f;  // 60フレーム
	const float holdDuration = 3.0f;	// 180フレーム
	const float shrinkDuration = 0.5f;  // 30フレーム

	float delta = 1.0f / 60.0f;

	switch (specialMoveState_) {
	case SpecialMoveState::kExpanding:
		specialMoveFrame_ += delta;
		{
			// 必殺技のフレーム管理
			float t = std::clamp(specialMoveFrame_ / expandDuration, 0.0f, 1.0f);
			t = Easing::EaseInQuint(t);
			// PostEffectへの値を適応
			postEffectManager_->GetGrayscaleData()->t = t;
			postEffectManager_->GetVignetteData()->gamma = t * 0.8f;
			// Cylinderのスケール、回転を適応
			float scale = t * 20.0f;
			cylinder_->GetTransform().scale = { scale ,scale / 2.0f ,scale };
			float angle = t * std::numbers::pi_v<float>;
			cylinder_->GetTransform().rotation = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::ExprUnitY, angle);

			if (specialMoveFrame_ >= expandDuration) {
				specialMoveFrame_ = 0.0f;
				specialMoveState_ = SpecialMoveState::kHolding;
				cylinder_->GetRenderOptions().enabled = false;
			}
		}
		break;

	case SpecialMoveState::kHolding:
		specialMoveFrame_ += delta;
		{
			// PostEffectへの値を適応
			postEffectManager_->GetGrayscaleData()->t = 1.0f;
			DeltaTimer::SetTimeScaleForSeconds(0.1f, 0.2f);

			if (Input::GetInstance()->GetGamepadLeftTrigger() == 0.0f &&
				specialMoveFrame_ >= holdDuration) {
				specialMoveFrame_ = 0.0f;
				specialMoveState_ = SpecialMoveState::kShrinking;
				cylinder_->GetRenderOptions().enabled = true;
			}
		}
		break;

	case SpecialMoveState::kShrinking:
		specialMoveFrame_ += delta;
		{
			// 必殺技のフレーム管理
			float t = std::clamp(1.0f - (specialMoveFrame_ / shrinkDuration), 0.0f, 1.0f);
			t = Easing::EaseInQuint(t);
			// PostEffectへの値を適応
			postEffectManager_->GetGrayscaleData()->t = t;
			postEffectManager_->GetVignetteData()->gamma = t * 0.8f;
			// Cylinderのスケール、回転を適応
			float scale = t * 20.0f;
			cylinder_->GetTransform().scale = { scale ,scale / 2.0f ,scale };
			float angle = t * std::numbers::pi_v<float>;
			cylinder_->GetTransform().rotation = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::ExprUnitY, angle);

			if (specialMoveFrame_ >= shrinkDuration) {
				specialMoveFrame_ = 0.0f;
				specialMoveState_ = SpecialMoveState::kNone;
				isSpecialMove_ = false;
				postEffectManager_->GetGrayscaleData()->t = 0.0f;
				postEffectManager_->GetVignetteData()->gamma = 0.0f;
				cylinder_->GetTransform().scale = {};
				cylinder_->GetRenderOptions().enabled = false;
				//player_->SpecialAttackBullet();
			}
		}
		break;

	case SpecialMoveState::kNone:
		if (isSpecialMove_) {
			specialMoveState_ = SpecialMoveState::kExpanding;
			cylinder_->GetRenderOptions().enabled = true;
			DeltaTimer::SetTimeScaleForSeconds(0.1f, 5.0f);
		}
		break;

	default:
		break;
	}
}
