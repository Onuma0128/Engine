#include "BaseEnemy.h"

#include "DeltaTimer.h"

#include "Objects/Player/Player.h"
#include "GameCamera/GameCamera.h"
#include "Collision/CollisionFilter.h"
#include "Objects/Enemy/AdjustItem/EnemyAdjustItem.h"

#include "Objects/Enemy/state/EnemyMoveState.h"
#include "Objects/Enemy/state/EnemyKnockbackState.h"
#include "Objects/Enemy/state/EnemyDeadState.h"
#include "Objects/Enemy/state/EnemyHitJumpState.h"
#include "Objects/Enemy/state/Melee/EnemyMelee_AttackState.h"
#include "Objects/Enemy/state/Ranged/EnemyRanged_AttackState.h"
#include "Objects/Enemy/state/ShieldBearer/EnemyShieldBearer_AttackState.h"
#include "Objects/Enemy/state/RangedElite/EnemyRangedElite_AttackState.h"

void BaseEnemy::Initialize()
{
	// 敵の視線を設定
	ray_ = std::make_unique<EnemyRay>();
	ray_->Init();
	
	// ステートを初期化
	ChangeState(std::make_unique<EnemyMoveState>(this));

	// エフェクトの初期化
	effect_ = std::make_unique<EnemyEffect>();
	effect_->SetEnemy(this);
	effect_->Init();

	// 探索アルゴリズムの初期化
	pathFinder_.Search(transform_.translation_, NumaEngine::Vector3{});

    // コライダーを設定
	NumaEngine::Collider::AddCollider();
	NumaEngine::Collider::myType_ = NumaEngine::ColliderType::kSphere;
	NumaEngine::Collider::colliderName_ = "Enemy";
	NumaEngine::Collider::size_ = transform_.scale_;
	NumaEngine::Collider::radius_ = transform_.scale_.x;
	NumaEngine::Collider::isActive_ = false;
	NumaEngine::Collider::targetColliderName_ = {
		"Player","MuscleCompanionAttack","FollowerMuscleCompanion","BlowDashMuscleCompanion",
		"PushUpMuscleCompanion",
		"Enemy" ,"PlayerShotRay","MuscleCompanion","SearchDashMuscleCompanion","EnemySearch",
	};
    NumaEngine::Collider::DrawCollider();

	// 基本的なアニメーションを設定
	transform_.translation_ = items_->GetMainData().startPosition;
    NumaEngine::Animation::SetSceneRenderer();
	NumaEngine::Animation::GetMaterial().enableDraw = false;
	NumaEngine::Animation::GetMaterial().outlineMask = true;
	NumaEngine::Animation::GetMaterial().outlineColor = NumaEngine::Vector3::ExprUnitX;
	NumaEngine::Animation::GetMaterial().shadowMap = false;
	NumaEngine::Animation::GetTimeStop() = true;
}

void BaseEnemy::Update()
{
	// レイの更新
	const float attackIn = GetTypeAttackDistance();
	NumaEngine::Vector3 direction = NumaEngine::Vector3::ExprUnitZ.Transform(NumaEngine::Quaternion::MakeRotateMatrix(transform_.rotation_));
	ray_->Update(transform_.translation_ + items_->GetMainData().rayOffset, direction * attackIn);

	// ステートの更新
	state_->Update();

	// エフェクトの更新
	effect_->Update();

    // 敵コライダーの更新
	NumaEngine::Collider::radius_ = items_->GetMainData().colliderSize;
	NumaEngine::Collider::centerPosition_ = transform_.translation_ + items_->GetMainData().colliderOffset;
	NumaEngine::Collider::Update();

	// アニメーションの更新
	Animation::Update();
}

void BaseEnemy::TransformUpdate()
{
	// 敵の行動許可が出ていなければ更新できない
    if (!stateParam_.enableMove_) {
		// オブジェクトの更新
		NumaEngine::Collider::Update();
		Animation::TransformUpdate();
	}
}

void BaseEnemy::ChangeState(std::unique_ptr<EnemyBaseState> newState)
{
	if (state_ != nullptr) {
		state_->Finalize();
	}
	state_ = std::move(newState);
	state_->Init();
}

void BaseEnemy::Dead()
{
	// ステートを初期化
	ChangeState(std::make_unique<EnemyMoveState>(this));
	// 描画とColliderを切る
	transform_.translation_ = items_->GetMainData().startPosition;
	Animation::GetMaterial().enableDraw = false;
	Animation::GetTimeStop() = true;
    NumaEngine::Collider::isActive_ = false;
	ray_->SetActive(false);
	ray_->Update(NumaEngine::Vector3::ExprZero, NumaEngine::Vector3::ExprUnitZ);
	// 影の描画も切る
	Animation::GetMaterial().shadowMap = false;
	stateParam_.enableMove_ = false;
}

void BaseEnemy::Reset(const NumaEngine::Vector3& position)
{
	// ステートを初期化
	ChangeState(std::make_unique<EnemyMoveState>(this));
	// 描画をする
	Animation::GetMaterial().enableDraw = true;
	Animation::GetTimeStop() = false;
	// 影を描画する
	Animation::GetMaterial().shadowMap = true;
	// 座標と回転を初期化する
	transform_.rotation_ = NumaEngine::Quaternion::IdentityQuaternion();
	transform_.translation_ = position;
	Animation::TransformUpdate();
	// ColliderをActiveに戻す
	Collider::isActive_ = true;
	ray_->SetActive(true);
	hitCollider_ = nullptr;
	stateParam_.isAlive_ = true;
	stateParam_.isDead_ = false;
	stateParam_.hitReticle_ = false;
	stateParam_.enableMove_ = true;
	stateParam_.isJumping_ = false;
}

void BaseEnemy::ResetSearch()
{
	if (hitCollider_) {
		pathFinder_.Search(transform_.translation_, hitCollider_->GetCenterPosition());
	} else {
		pathFinder_.Search(transform_.translation_, player_->GetTransform().translation_);
	}
}


void BaseEnemy::OnCollisionEnter(Collider* other)
{
	// 判定用フラグ
	bool isCompanion = other->GetColliderName() == "MuscleCompanion";
	bool isPushUpCompanion = other->GetColliderName() == "PushUpMuscleCompanion";
	bool isCompanionAttack = other->GetColliderName() == "MuscleCompanionAttack";
	bool isSearchDashCompanion = other->GetColliderName() == "SearchDashMuscleCompanion";
	bool isFollowerCompanion = other->GetColliderName() == "FollowerMuscleCompanion";
	bool isBlowDashCompanion = other->GetColliderName() == "BlowDashMuscleCompanion";

	// プレイヤーの仲間と当たっているなら
	if (CollisionFilter::CheckColliderNameCompanion(other->GetColliderName())) {
		// 小さな当たり判定は無視する
		if (other->GetRadius() < 0.5f || isPushUpCompanion || transform_.translation_.y >= 0.2f) {
			return;
		}
		// 初回ヒット時はヒットジャンプステートに遷移
		if (isCompanion && other->GetRadius() > 0.5f && !stateParam_.isJumping_) {
            NumaEngine::DeltaTimer::SetTimeScaleForSeconds(0.1f, 0.1f);
			stateParam_.isJumping_ = true;
			ChangeState(std::make_unique<EnemyHitJumpState>(this));
		} else if (isFollowerCompanion || isSearchDashCompanion) {
			velocity_ = NumaEngine::Vector3{ transform_.translation_ - player_->GetTransform().translation_}.Normalize();
			ChangeState(std::make_unique<EnemyKnockbackState>(this));
		}
		if (!hitCollider_ && (isCompanion || isCompanionAttack)) {
			hitCollider_ = other;
		}
		// ダメージ処理
		if (isBlowDashCompanion) {
			currentHp_ = 0;
		} else {
			if (currentHp_ > 0) {
				if (currentHp_ >= GetTypeUsuallyDamage()) {
					currentHp_ -= GetTypeUsuallyDamage();
				} else {
					currentHp_ = 0;
				}
				if (other->GetRadius() > 1.5f && currentHp_ > 0) { 
					if (currentHp_ >= GetTypePowerUpDamage()) {
						currentHp_ -= GetTypePowerUpDamage();
					} else {
						currentHp_ = 0;
					}
				}
			}
		}
		// エフェクトを描画
		WorldTransform transform;
		transform.rotation_ = other->GetRotate();
		transform.translation_ = transform_.translation_;
		effect_->OnceBulletHitEffect(transform);
		transform.rotation_ = transform_.rotation_;
		transform.translation_ = transform_.translation_ - (velocity_ * 0.5f);
		if (isSearchDashCompanion) {
			effect_->OnceBulletHitExplosionBlueEffect(transform);
		} else {
			effect_->OnceBulletHitExplosionEffect(transform);
		}
		// 体力が0以下なら死亡処理へ
		if (currentHp_ <= 0) {
        // Colliderを無効化する
		this->isActive_ = false;
			ray_->SetActive(false);
			stateParam_.isAlive_ = false;
			// 敵がノックバックする方向を取得
        NumaEngine::Matrix4x4 rotate = NumaEngine::Quaternion::MakeRotateMatrix(other->GetRotate());
			velocity_ = NumaEngine::Vector3::ExprUnitZ.Transform(rotate);
			playerBulletPosition_ = other->GetCenterPosition();
			// 死亡時のステートに遷移
			ChangeState(std::make_unique<EnemyDeadState>(this));
		}
	}
}

void BaseEnemy::OnCollisionStay(Collider* other)
{
	// プレイヤーと当たっているなら
	if (other->GetColliderName() == "Player") {
		const float speed = 2.0f;
        transform_.translation_ -= velocity_ * speed * NumaEngine::DeltaTimer::GetDeltaTime();
	}

	// 敵と当たっているなら
	if (other->GetColliderName() == "Enemy") {
		const float speed = 1.0f;
		NumaEngine::Vector3 velocity = transform_.translation_ - other->GetCenterPosition();
		velocity.y = 0.0f;
		if (velocity.Length() != 0.0f) { velocity = velocity.Normalize(); }
        transform_.translation_ += velocity * speed * NumaEngine::DeltaTimer::GetDeltaTime();
	}
}

void BaseEnemy::OnCollisionExit(Collider* other)
{
}

void BaseEnemy::TypeChengeAttackState()
{
	switch (type_) {
	case EnemyType::kMelee:			ChangeState(std::make_unique<EnemyMelee_AttackState>(this)); break;
	case EnemyType::kRanged:		ChangeState(std::make_unique<EnemyRanged_AttackState>(this)); break;
	case EnemyType::kShieldBearer:	ChangeState(std::make_unique<EnemyShieldBearer_AttackState>(this)); break;
	case EnemyType::kRangedElite:	ChangeState(std::make_unique<EnemyRangedElite_AttackState>(this)); break;
	default:break;
	}
}

const float BaseEnemy::GetTypeAttackDistance()
{
	switch (type_) {
	case EnemyType::kMelee:			return items_->GetMeleeData().tempData.attackDistance;
	case EnemyType::kRanged:		return items_->GetRangedData().tempData.attackDistance;
	case EnemyType::kShieldBearer:	return items_->GetShieldBearerData().tempData.attackDistance;
	case EnemyType::kRangedElite:	return items_->GetRangedEliteData().tempData.attackDistance;
	default:break;
	}
	return 0.0f;
}

const uint32_t BaseEnemy::GetTypeUsuallyDamage()
{
	switch (type_) {
	case EnemyType::kMelee:			return static_cast<uint32_t>(items_->GetMeleeData().tempData.usuallyDamage);
	case EnemyType::kRanged:		return static_cast<uint32_t>(items_->GetRangedData().tempData.usuallyDamage);
	case EnemyType::kShieldBearer:	return static_cast<uint32_t>(items_->GetShieldBearerData().tempData.usuallyDamage);
	case EnemyType::kRangedElite:	return static_cast<uint32_t>(items_->GetRangedEliteData().tempData.usuallyDamage);
	default:break;
	}
	return 0u;
}

const uint32_t BaseEnemy::GetTypePowerUpDamage()
{
	switch (type_) {
	case EnemyType::kMelee:			return static_cast<uint32_t>(items_->GetMeleeData().tempData.powerUpDamage);
	case EnemyType::kRanged:		return static_cast<uint32_t>(items_->GetRangedData().tempData.powerUpDamage);
	case EnemyType::kShieldBearer:	return static_cast<uint32_t>(items_->GetShieldBearerData().tempData.powerUpDamage);
	case EnemyType::kRangedElite:	return static_cast<uint32_t>(items_->GetRangedEliteData().tempData.powerUpDamage);
	default:break;
	}
	return 0u;
}

void BaseEnemy::HitColliderNotActive()
{
	if (hitCollider_ && !hitCollider_->GetActive()) {
		hitCollider_ = nullptr;
	}
}

