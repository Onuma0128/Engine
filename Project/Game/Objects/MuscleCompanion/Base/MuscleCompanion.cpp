#include "MuscleCompanion.h"

#include <numbers>
#include "Input.h"
#include "DeltaTimer.h"

#include "Collision/CollisionFilter.h"
#include "Objects/Player/Player.h"
#include "SearchAlgorithm/Collision/MapCollision.h"

#include "Objects/MuscleCompanion/State/CompanionIdleState.h"
#include "Objects/MuscleCompanion/State/CompanionMoveState.h"
#include "Objects/MuscleCompanion/State/CompanionAttackState.h"
#include "Objects/MuscleCompanion/State/CompanionDeadState.h"
#include "Objects/MuscleCompanion/State/CompanionKnockbackState.h"

void MuscleCompanion::Initialize()
{
	// 基本的なアニメーションを設定
	Animation::Initialize("Mattyo.gltf");
	Animation::SetSceneRenderer();
	Animation::PlayByName("Wait");
	Animation::GetMaterial().outlineMask = true;
	Animation::GetMaterial().outlineColor = Vector3::ExprZero;

	// コライダーを設定
	Collider::AddCollider();
	Collider::myType_ = ColliderType::kSphere;
	Collider::colliderName_ = "MuscleCompanion";
	Collider::size_ = transform_.scale_;
	Collider::radius_ = transform_.scale_.x;
	Collider::isActive_ = true;
	Collider::targetColliderName_ = { 
		"MuscleCompanion","Enemy","BossEnemy" ,"EnemyRay","BossAttack",
		"EnemyMelee","EnemyShieldBearer","EnemyRanged","EnemyRangedElite",
		"Building","DeadTree","fence","Bush","StoneWall","ShortStoneWall",
	};
	Collider::DrawCollider();

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

	// ダッシュ時の一回目の攻撃フラグの初期化
	isFirstDashAttack_ = true;
	// 集合要求フラグの初期化
	isGatherRequested_ = true;
	// 元の場所に戻ったかのフラグの初期化
	isReturnOriginal_ = true;
	// HPの初期化
	maxHp_ = items_->GetMainData().maxHP;
	currentHp_ = maxHp_;
}

void MuscleCompanion::Update()
{
	// ステートの更新
	state_->Update();

	// カラーを点滅させる
	BlinkingColor();

	// コライダーの更新
	attackCollider_->Update();
	followerCollider_->Update();
	Collider::radius_ = items_->GetMainData().colliderSize * colliderScale_;
	Collider::centerPosition_ = transform_.translation_ + items_->GetMainData().colliderOffset;
	Collider::Update();

	// アニメーションの更新
	Animation::SetTransformScale(Vector3::ExprUnitXYZ * items_->GetMainData().objectScale);
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

	// 建物に当たったら待機状態へ
	if (CollisionFilter::CheckColliderNameFieldObject(other->GetColliderName()) && isDash) {
		ChangeState(std::make_unique<CompanionIdleState>(this));
		// 敵に当たったら攻撃状態へ
	} else if (other->GetColliderName() == "Enemy" || other->GetColliderName() == "BossEnemy") {
		if (Collider::radius_ > 0.6f && (isDash || isSearchDash)) {
			isFirstDashAttack_ = true;
			Input::GetInstance()->Vibrate(0.4f, 0.75f, 100);
			Vector3 velocity = other->GetCenterPosition() - transform_.translation_;
			Quaternion yRotation_ = Quaternion::DirectionToQuaternion(
				transform_.rotation_, velocity.Normalize(), 1.0f);
			transform_.rotation_ = yRotation_;
			ChangeState(std::make_unique<CompanionAttackState>(this));
		}
	}
	// 敵に当たったら効果音を鳴らす
	if ((other->GetColliderName() == "Enemy" || other->GetColliderName() == "BossEnemy") && isMove) {
		if (Collider::radius_ > 0.6f) {
			const auto& volume = items_->GetSeVolumeData();
			audio_->SoundPlayWave("MattyoGiveDamage.wav", volume.giveDamage);
		}
	}
	// 敵の攻撃に当たったら体力を1減らす
	if (CollisionFilter::CheckColliderNameEnemy(other->GetColliderName())) {
		--currentHp_;
		effect_->OnceHitEffect();
		const auto& volume = items_->GetSeVolumeData();
		audio_->SoundPlayWave("MattyoGetDamage.wav", volume.getDamage);
		if (currentHp_ <= 0) {
			ChangeState(std::make_unique<CompanionDeadState>(this));
		} else if (other->GetColliderName() == "BossAttack") {
			knockbackDirection_ = transform_.translation_ - other->GetCenterPosition();
			ChangeState(std::make_unique<CompanionKnockbackState>(this));
		}
	}
}

void MuscleCompanion::OnCollisionStay(Collider* other)
{
	// 死亡状態なら何もしない
	if (state_->GetState() == CharacterState::Dead) {
		return;
	}

	// 仲間と当たっているなら
	if (other->GetColliderName() == "MuscleCompanion") {
		// プレイヤーとの距離を測る
		const auto dist1 = Vector3::Distance(transform_.translation_, player_->GetTransform().translation_);
		const auto dist2 = Vector3::Distance(other->GetCenterPosition(), player_->GetTransform().translation_);
		Vector3 velocity = transform_.translation_ - other->GetCenterPosition();
		velocity.y = 0.0f;
		if (velocity.Length() != 0.0f) { velocity = velocity.Normalize(); }

		// 当たった仲間より近かったらプレイヤーに近づける
		if (dist1 < dist2 && !SearchDistance()) {
			const float speed = items_->GetMainData().hitPushSpeed;
			transform_.translation_ += velocity * speed * DeltaTimer::GetDeltaTime();
		} else if (SearchDistance()) {
			const float speed = 1.0f;
			transform_.translation_ += velocity * speed * DeltaTimer::GetDeltaTime();
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
		blinkingTime_ += DeltaTimer::GetDeltaTime() * scale;
		blinkingTime_ = std::clamp(blinkingTime_, 0.0f, std::numbers::pi_v<float>);
		float color = std::sinf(blinkingTime_) + (data.blinkingColor / scale);
		color = std::clamp(color, 0.0f, 1.0f);
		Animation::GetMaterial().color = { 1.0f,color,color,1.0f };
		if (blinkingTime_ >= std::numbers::pi_v<float>) {
			blinkingTime_ = 0.0f;
		}
	} else {
		Animation::GetMaterial().color = Vector4::ExprUnitXYZW;
	}
}

void MuscleCompanion::ChangeState(std::unique_ptr<CompanionBaseState> newState)
{
	if (state_ != nullptr) {
		state_->Finalize();
	}
	state_ = std::move(newState);
	state_->Init();
}

void MuscleCompanion::ResetSearch(const Vector3& goalPosition)
{
	pathFinder_.Search(transform_.translation_, goalPosition);
}

bool MuscleCompanion::SearchDistance()
{
	// 距離が遠ければtrueを返す
	const float dist = Vector3::Distance(transform_.translation_, player_->GetTransform().translation_);
	if (dist > items_->GetMainData().searchCancelDistance) { return true; }
	return false;
}
