#include "MuscleCompanion.h"

#include <numbers>
#include "Input.h"
#include "DeltaTimer.h"

#include "Collision/CollisionFilter.h"
#include "Objects/Player/Player.h"
#include "SearchAlgorithm/Collision/MapCollision.h"

#include "Objects/MuscleCompanion/State/CompanionPushUpIdleState.h"
#include "Objects/MuscleCompanion/State/CompanionMoveState.h"
#include "Objects/MuscleCompanion/State/CompanionAttackState.h"
#include "Objects/MuscleCompanion/State/CompanionDeadState.h"
#include "Objects/MuscleCompanion/State/CompanionKnockbackState.h"
#include "Objects/MuscleCompanion/State/CompanionShieldKnockbackState.h"

void MuscleCompanion::Initialize()
{
	// 基本的なアニメーションを設定
	Animation::Initialize("Mattyo.gltf");
	Animation::SetSceneRenderer();
	Animation::PlayByName("Wait");
	Animation::SetTransformScale(NumaEngine::Vector3::ExprUnitXYZ * items_->GetMainData().objectScale);
	Animation::GetMaterial().outlineMask = true;
	Animation::GetMaterial().outlineColor = NumaEngine::Vector3::ExprZero;

    // コライダーを設定
	NumaEngine::Collider::AddCollider();
	NumaEngine::Collider::myType_ = NumaEngine::ColliderType::kSphere;
	NumaEngine::Collider::colliderName_ = "MuscleCompanion";
	NumaEngine::Collider::size_ = transform_.scale_;
	NumaEngine::Collider::radius_ = transform_.scale_.x;
	NumaEngine::Collider::isActive_ = true;
	NumaEngine::Collider::targetColliderName_ = { 
		"MuscleCompanion","Enemy","BossEnemy" ,"EnemyRay","BossAttack",
		"EnemyMelee","EnemyShieldBearer","EnemyRanged","EnemyRangedElite",
		"Building","DeadTree","fence","Bush","StoneWall","ShortStoneWall",
		"SearchDashMuscleCompanion","PushUpMuscleCompanion","EnemyShield","EnemyBulletRay",
	};
    NumaEngine::Collider::DrawCollider();

	// ステートの初期化
	ChangeState(std::make_unique<CompanionMoveState>(this));

	// コライダーの初期化
	attackCollider_ = std::make_unique<CompanionAttackCollider>();
	attackCollider_->SetCompanion(this);
	attackCollider_->Initialize();
	followerCollider_ = std::make_unique<CompanionFollowerCollider>();
	followerCollider_->SetCompanion(this);
	followerCollider_->Initialize();

	// エフェクトの初期化
	effect_ = std::make_unique<CompanionEffect>();
	effect_->SetCompanion(this);
	effect_->Init();

	// 初期化
	Reset();
}

void MuscleCompanion::Update()
{
	// ステートの更新
	state_->Update();

	// エフェクトの更新
	effect_->Update();

	// カラーを点滅させる
	BlinkingColor();

	// レベルアップの処理
	LevelUp();

	// コライダーの更新
    attackCollider_->Update();
	followerCollider_->Update();
    NumaEngine::Collider::radius_ = transform_.scale_.x * items_->GetMainData().colliderSize * colliderScale_;
	NumaEngine::Collider::centerPosition_ = transform_.translation_ + items_->GetMainData().colliderOffset;
	NumaEngine::Collider::Update();

	// アニメーションの更新
	ApplyScaleByLevel();
	Animation::Update();
}

void MuscleCompanion::Draw()
{
	state_->Draw();

	effect_->Draw();
}

void MuscleCompanion::OnCollisionEnter(Collider* other)
{
	// 死亡状態なら何もしない
	if(state_->GetState() == CharacterState::Dead){
		return;
	}

	// ダッシュ状態で当たったら
	bool isDash = state_->GetState() == CharacterState::Dash;
	bool isSearchDash = state_->GetState() == CharacterState::SearchDash;
	bool isMove = state_->GetState() == CharacterState::Move;

	// 進行ベクトルを取り背面の建物を判定する
	const auto& data = items_->GetDashData();
	NumaEngine::Vector3 toTarget = other->GetCenterPosition() - transform_.translation_;
	if (dashDirection_.Length() <= 0.0f || toTarget.Length() < 0.0f) { return; }
	float dot = NumaEngine::Vector3::Dot(dashDirection_.Normalize(), toTarget.Normalize());
	dot = std::clamp(dot, -1.0f, 1.0f);
	// 角度を計算して、背面に建物があるか判定する
	std::string name = other->GetColliderName();
	float angle = std::acos(dot) * 180.0f / std::numbers::pi_v<float>;
	bool isBack = angle >= data.dashMinBackAngle && angle <= data.dashMaxBackAngle;
	// 建物に当たったら待機状態へ
	if (CollisionFilter::CheckColliderNameFieldObject(other->GetColliderName()) && isDash && !isBack) {
		ChangeState(std::make_unique<CompanionPushUpIdleState>(this));
	// 敵に当たったら攻撃状態へ
	} else if (other->GetColliderName() == "Enemy" || other->GetColliderName() == "BossEnemy") {
        if (NumaEngine::Collider::radius_ > 0.6f && (isDash || isSearchDash)) {
			isFirstDashAttack_ = true;
			Input::GetInstance()->Vibrate(0.4f, 0.75f, 100);
			NumaEngine::Vector3 velocity = other->GetCenterPosition() - transform_.translation_;
			NumaEngine::Quaternion yRotation_ = NumaEngine::Quaternion::DirectionToQuaternion(
				transform_.rotation_, velocity.Normalize(), 1.0f);
			transform_.rotation_ = yRotation_;
			ChangeState(std::make_unique<CompanionAttackState>(this));
		}
	}
	// 敵に当たったら効果音を鳴らす
    if ((other->GetColliderName() == "Enemy" || other->GetColliderName() == "BossEnemy") && isMove) {
        if (NumaEngine::Collider::radius_ > 0.6f) {
			const auto& volume = items_->GetSeVolumeData();
			audio_->SoundPlayWave("MattyoGiveDamage.wav", volume.giveDamage);
		}
	}
	// 敵の攻撃に当たったら体力を1減らす
	if (CollisionFilter::CheckColliderNameEnemy(other->GetColliderName())) {
		if (!isInvincible_) { --currentHp_; }
		effect_->OnceHitEffect();
		const auto& volume = items_->GetSeVolumeData();
		audio_->SoundPlayWave("MattyoGetDamage.wav", volume.getDamage);
		if (currentHp_ <= 0) {
			ChangeState(std::make_unique<CompanionDeadState>(this));
		} else if (other->GetColliderName() == "BossAttack") {
			knockbackRotate_ = other->GetRotate();
			knockbackPosition_ = other->GetCenterPosition();
			ChangeState(std::make_unique<CompanionKnockbackState>(this));
		}
	}
	// 敵のシールドに当たったら
	if (other->GetColliderName() == "EnemyShield") {
		const auto& volume = items_->GetSeVolumeData();
		audio_->SoundPlayWave("MattyoShield.wav", volume.shield);
		knockbackRotate_ = other->GetRotate();
		knockbackPosition_ = other->GetCenterPosition();
		ChangeState(std::make_unique<CompanionShieldKnockbackState>(this));
	}
}

void MuscleCompanion::OnCollisionStay(Collider* other)
{
	// 死亡状態なら何もしない
	if (state_->GetState() == CharacterState::Dead) {
		return;
	}

	bool isCompanion = 
		other->GetColliderName() == "MuscleCompanion" || 
		other->GetColliderName() == "SearchDashMuscleCompanion" ||
		other->GetColliderName() == "PushUpMuscleCompanion";
	bool isKnockback = state_->GetState() == CharacterState::ShieldKnockback;

	// 仲間と当たっているなら
	if (isCompanion) {
		// プレイヤーとの距離を測る
		const auto dist1 = NumaEngine::Vector3::Distance(transform_.translation_, player_->GetTransform().translation_);
		const auto dist2 = NumaEngine::Vector3::Distance(other->GetCenterPosition(), player_->GetTransform().translation_);
		NumaEngine::Vector3 velocity = transform_.translation_ - other->GetCenterPosition();
		velocity.y = 0.0f;
		if (velocity.Length() != 0.0f) { velocity = velocity.Normalize(); }

		// 当たった仲間より近かったらプレイヤーに近づける
		if (dist1 < dist2 && !SearchDistance()) {
			const float speed = items_->GetMainData().hitPushSpeed;
            transform_.translation_ += velocity * speed * NumaEngine::DeltaTimer::GetDeltaTime();
		} else if (SearchDistance()) {
			const float speed = 1.0f;
            transform_.translation_ += velocity * speed * NumaEngine::DeltaTimer::GetDeltaTime();
		}
	}

	if (isKnockback) {
		if (CollisionFilter::CheckColliderNameFieldObject(other->GetColliderName())) {
			ChangeState(std::make_unique<CompanionPushUpIdleState>(this));
		}
	}
}

void MuscleCompanion::OnCollisionExit(Collider* other)
{
}

void MuscleCompanion::BlinkingColor()
{
	const auto& data = items_->GetMainData();
	if (static_cast<uint32_t>(data.blinkingHP) > currentHp_ && currentHp_ > 0) {
		float scale = static_cast<float>(data.blinkingHP) / static_cast<float>(currentHp_);
		float timeScale = data.blinkingTimeScale * scale;
        blinkingTime_ += NumaEngine::DeltaTimer::GetDeltaTime() * timeScale;
		blinkingTime_ = std::clamp(blinkingTime_, 0.0f, std::numbers::pi_v<float>);
		float color = std::sinf(blinkingTime_) + (data.blinkingColor / scale);
		color = std::clamp(color, 0.0f, 1.0f);
		Animation::GetMaterial().color = { 1.0f,color,color,1.0f };
		if (blinkingTime_ >= std::numbers::pi_v<float>) {
			blinkingTime_ = 0.0f;
		}
	} else {
		Animation::GetMaterial().color = NumaEngine::Vector4::ExprUnitXYZW;
	}
}

void MuscleCompanion::ApplyScaleByLevel()
{
	const auto& data = items_->GetPushUpData();
	level_ = std::clamp(static_cast<int>(level_), 0, data.maxLevel - 1);
	float scale = data.objectScale[level_];
	Animation::SetTransformScale(NumaEngine::Vector3::ExprUnitXYZ * scale);
}

void MuscleCompanion::LevelUp()
{
	if (level_ != prevLevel_) { 
		levelUpEffectTime_ = 0.0f;
		effect_->OnceLevelUpRingEffect();
	}

	if (level_ != prevLevel_ || levelUpEffectTime_ > 0.0f) {
		const auto& data = items_->GetPushUpData();
		float effectTime = data.effectTime;
    levelUpEffectTime_ += NumaEngine::DeltaTimer::GetDeltaTime();
		effect_->LevelUpEffect(true);
		if (levelUpEffectTime_ > effectTime) {
			effect_->LevelUpEffect(false);
			levelUpEffectTime_ = 0.0f;
		}
	}

	// レベルを保存する
	prevLevel_ = level_;
}

void MuscleCompanion::ChangeState(std::unique_ptr<CompanionBaseState> newState)
{
	if (state_ != nullptr) {
		state_->Finalize();
	}
	state_ = std::move(newState);
	state_->Init();
}

void MuscleCompanion::ResetSearch(const NumaEngine::Vector3& goalPosition)
{
	pathFinder_.Search(transform_.translation_, goalPosition);
}

bool MuscleCompanion::SearchDistance()
{
	// 距離が遠ければtrueを返す
	const float dist = NumaEngine::Vector3::Distance(transform_.translation_, player_->GetTransform().translation_);
	if (dist > items_->GetMainData().searchCancelDistance) { return true; }
	return false;
}

void MuscleCompanion::Reset(bool levelReset)
{
	// コライダーの初期化
	Collider::colliderName_ = "MuscleCompanion";
	Collider::isActive_ = true;
	colliderScale_ = 1.0f;

	// ダッシュ時の一回目の攻撃フラグの初期化
	isFirstDashAttack_ = true;
	// 集合要求フラグの初期化
	isGatherRequested_ = true;
	// 元の場所に戻ったかのフラグの初期化
	isReturnOriginal_ = true;
	// HPの初期化
	maxHp_ = items_->GetMainData().maxHP;
	currentHp_ = maxHp_;
	// 経験値
	if (levelReset) {
		level_ = 0;
		experience_ = 0.0f;
	}
	// スケールの初期化
	Animation::SetTransformScale(NumaEngine::Vector3::ExprUnitXYZ * items_->GetMainData().objectScale);

	// ステートの初期化
	ChangeState(std::make_unique<CompanionMoveState>(this));
}
