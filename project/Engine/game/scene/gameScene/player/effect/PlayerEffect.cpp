#include "PlayerEffect.h"

#include "DirectXEngine.h"
#include "PostEffectManager.h"
#include "DeltaTimer.h"
#include "Input.h"

#include "Easing.h"
#include "gameScene/player/Player.h"

void PlayerEffect::Init()
{
	// 移動時のエフェクト
	moveDustEmitter_ = std::make_unique<ParticleEmitter>("moveDust");
	particleManager_->CreateParticleGroup("moveDust", "smoke.png", moveDustEmitter_.get());
	moveDustEmitter_->SetIsCreate(false);

	// 弾のエフェクト
	for (size_t i = 0; i < bulletTrailEmitters_.size(); ++i) {
		bulletTrailEmitters_[i] = std::make_unique<ParticleEmitter>("bulletTrail");
		particleManager_->CreateParticleGroup("bulletTrail" + std::to_string(i), "white1x1.png", bulletTrailEmitters_[i].get());
		bulletTrailEmitters_[i]->SetIsCreate(false);

		bulletDeleteEmitters_[i] = std::make_unique<ParticleEmitter>("bulletDelete");
		particleManager_->CreateParticleGroup("bulletDelete" + std::to_string(i), "white1x1.png", bulletDeleteEmitters_[i].get());
		bulletDeleteEmitters_[i]->SetIsCreate(false);
	}

	// 弾を撃つ時のエフェクト
	bulletExplosionEmitter_ = std::make_unique<ParticleEmitter>("bulletExplosion");
	particleManager_->CreateParticleGroup("bulletExplosion", "circle.png", bulletExplosionEmitter_.get());
	bulletExplosionEmitter_->SetIsCreate(false);

	bulletSparkEmitter_ = std::make_unique<ParticleEmitter>("bulletSpark");
	particleManager_->CreateParticleGroup("bulletSpark", "circle.png", bulletSparkEmitter_.get());
	bulletSparkEmitter_->SetIsCreate(false);

	bulletSmokeEmitter_ = std::make_unique<ParticleEmitter>("bulletSmoke");
	particleManager_->CreateParticleGroup("bulletSmoke", "smoke.png", bulletSmokeEmitter_.get());
	bulletSmokeEmitter_->SetIsCreate(false);

	bulletCartridgeEmitter_ = std::make_unique<ParticleEmitter>("bulletCartridge");
	particleManager_->CreateParticleGroup("bulletCartridge", "white1x1.png", bulletCartridgeEmitter_.get());
	bulletCartridgeEmitter_->SetIsCreate(false);

	// 避け時のエフェクト
	avoidDustEmitter_ = std::make_unique<ParticleEmitter>("avoidDust");
	particleManager_->CreateParticleGroup("avoidDust", "smoke.png", avoidDustEmitter_.get());
	avoidDustEmitter_->SetIsCreate(false);

	// PostEffectを初期化
	DirectXEngine::GetPostEffectMgr()->CreatePostEffect(PostEffectType::Grayscale);
	DirectXEngine::GetPostEffectMgr()->CreatePostEffect(PostEffectType::Vignette);

	cylinder_ = std::make_unique<PrimitiveDrawr>();
	cylinder_->TypeInit(PrimitiveType::Cylinder, 32);
	cylinder_->GetTransform().scale = {};
	cylinder_->SetColor({ 1.0f,1.0f,0.0f });
	cylinder_->GetRenderOptions().enabled = false;
	cylinder_->GetRenderOptions().offscreen = false;
}

void PlayerEffect::Update()
{	
	UpdatePostEffect();

	cylinder_->GetTransform().translation = player_->GetTransform().translation_;
	cylinder_->GetTransform().translation.y = 0.0f;
	cylinder_->Update();
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

void PlayerEffect::OnceBulletTrailEffect(const int32_t count, const WorldTransform& transform)
{
	bulletTrailEmitters_[count]->onceEmit();

	// パーティクルの座標を設定
	Quaternion rotate = transform.rotation_;
	Vector3 position = transform.translation_;

	bulletTrailEmitters_[count]->SetRotation(rotate);
	bulletTrailEmitters_[count]->SetPosition(position);
}

void PlayerEffect::OnceBulletDeleteEffect(const int32_t count, const WorldTransform& transform)
{
	bulletDeleteEmitters_[count]->onceEmit();

	// パーティクルの座標を設定
	Quaternion rotate = transform.rotation_;
	Vector3 position = transform.translation_;

	bulletDeleteEmitters_[count]->SetRotation(rotate);
	bulletDeleteEmitters_[count]->SetPosition(position);
}

void PlayerEffect::OnceBulletEffect()
{
	// Particleを一回生成
	bulletExplosionEmitter_->onceEmit();
	bulletSparkEmitter_->onceEmit();
	bulletSmokeEmitter_->onceEmit();
	bulletCartridgeEmitter_->onceEmit();

	// パーティクルの座標を設定
	Quaternion rotate = player_->GetTransform().rotation_;
	Vector3 position = player_->GetTransform().translation_;

	// 爆発
	bulletExplosionEmitter_->SetPosition(position);
	bulletExplosionEmitter_->SetRotation(rotate);
	// 火花
	bulletSparkEmitter_->SetPosition(position);
	bulletSparkEmitter_->SetRotation(rotate);
	// 煙
	bulletSmokeEmitter_->SetPosition(position);
	bulletSmokeEmitter_->SetRotation(rotate);
	// 薬莢
	bulletCartridgeEmitter_->SetPosition(position);
	bulletCartridgeEmitter_->SetRotation(rotate);
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

void PlayerEffect::UpdatePostEffect()
{
	const float expandDuration = 1.0f;  // 60フレーム
	const float holdDuration = 3.0f;	// 180フレーム
	const float shrinkDuration = 1.0f;  // 60フレーム

	float delta = DeltaTimer::GetDeltaTime();

	switch (specialMoveState_) {
	case SpecialMoveState::Expanding:
		specialMoveFrame_ += delta;
		{
			// 必殺技のフレーム管理
			float t = std::clamp(specialMoveFrame_ / expandDuration, 0.0f, 1.0f);
			t = Easing::EaseInQuint(t);
			// PostEffectへの値を適応
			DirectXEngine::GetPostEffectMgr()->GetGrayscaleData()->t = t;
			DirectXEngine::GetPostEffectMgr()->GetVignetteData()->gamma = t * 0.8f;
			// Cylinderのスケール、回転を適応
			float scale = t * 20.0f;
			cylinder_->GetTransform().scale = { scale ,scale / 2.0f ,scale };
			cylinder_->GetTransform().rotation.y = t * 3.14f;

			if (specialMoveFrame_ >= expandDuration) {
				specialMoveFrame_ = 0.0f;
				specialMoveState_ = SpecialMoveState::Holding;
				cylinder_->GetRenderOptions().enabled = false;
			}
		}
		break;

	case SpecialMoveState::Holding:
		specialMoveFrame_ += delta;
		{
			// PostEffectへの値を適応
			DirectXEngine::GetPostEffectMgr()->GetGrayscaleData()->t = 1.0f;

			if (Input::GetInstance()->GetGamepadRightTrigger() == 0.0f &&
				specialMoveFrame_ >= holdDuration) {
				specialMoveFrame_ = 0.0f;
				specialMoveState_ = SpecialMoveState::Shrinking;
				cylinder_->GetRenderOptions().enabled = true;
			}
		}
		break;

	case SpecialMoveState::Shrinking:
		specialMoveFrame_ += delta;
		{
			// 必殺技のフレーム管理
			float t = std::clamp(1.0f - (specialMoveFrame_ / shrinkDuration), 0.0f, 1.0f);
			t = Easing::EaseInQuint(t);
			// PostEffectへの値を適応
			DirectXEngine::GetPostEffectMgr()->GetGrayscaleData()->t = t;
			DirectXEngine::GetPostEffectMgr()->GetVignetteData()->gamma = t * 0.8f;
			// Cylinderのスケール、回転を適応
			float scale = t * 20.0f;
			cylinder_->GetTransform().scale = { scale ,scale / 2.0f ,scale };
			cylinder_->GetTransform().rotation.y = t * 3.14f;

			if (specialMoveFrame_ >= shrinkDuration) {
				specialMoveFrame_ = 0.0f;
				specialMoveState_ = SpecialMoveState::None;
				isSpecialMove_ = false;
				DirectXEngine::GetPostEffectMgr()->GetGrayscaleData()->t = 0.0f;
				DirectXEngine::GetPostEffectMgr()->GetVignetteData()->gamma = 0.0f;
				cylinder_->GetTransform().scale = {};
				cylinder_->GetRenderOptions().enabled = false;
			}
		}
		break;

	case SpecialMoveState::None:
		if (isSpecialMove_) {
			specialMoveState_ = SpecialMoveState::Expanding;
			cylinder_->GetRenderOptions().enabled = true;
		}
		break;

	default:
		break;
	}
}
