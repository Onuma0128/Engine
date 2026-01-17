#include "BossEnemy.h"

#include "DeltaTimer.h"

#include "Collision/CollisionFilter.h"

#include "Objects/Boss/State/BossIdleState.h"
#include "Objects/Boss/State/BossAppearState.h"
#include "Objects/Boss/State/BossMoveState.h"
#include "Objects/Boss/State/BossDownState.h"
#include "Objects/Boss/State/BossMeleeState.h"
#include "Objects/Boss/State/BossDeadState.h"

#include "Objects/Player/Player.h"
#include "Objects/Enemy/Spawner/EnemySpawnerFactory.h"

void BossEnemy::Initialize()
{
	// 調整項目の設定
	items_ = std::make_unique<BossAdjustItem>();
	items_->LoadItems();

	// Audioの初期化
	audio_ = std::make_unique<Audio>();

	// アニメーションを設定
	Animation::Initialize("BossEnemy.gltf");
	Animation::SetSceneRenderer();
	Animation::PlayByName("Idle", 0.0f);
	Animation::GetMaterial().enableDraw = false;
	Animation::GetMaterial().outlineMask = false;
	Animation::GetMaterial().outlineColor = Vector3::ExprUnitX;
	Animation::SetTransformTranslation(items_->GetMainData().startPosition);

	// コライダーを設定
	Collider::AddCollider();
	Collider::isActive_ = false;
	Collider::colliderName_ = "BossEnemy";
	Collider::myType_ = ColliderType::kSphere;
	Collider::targetColliderName_ = {
		"Player","MuscleCompanion","PlayerShotRay","MuscleCompanionAttack","SearchDashMuscleCompanion",
		"Building","DeadTree","fence","Bush","StoneWall","ShortStoneWall","BlowDashMuscleCompanion",
	};
	Collider::DrawCollider();

	// HPの初期化
	maxHp_ = items_->GetMainData().maxHP;
	currentHp_ = maxHp_;

	// 攻撃用コライダーを設定
	attackCollider_ = std::make_unique<BossAttackCollider>();
	attackCollider_->SetBossEnemy(this);
	attackCollider_->Initialize();

	// ボスの視線を設定
	ray_ = std::make_unique<BossRay>();
	ray_->Init();
	ray_->SetActive(false);

	// ステート計算の初期化
	stateEvaluator_ = std::make_unique<BossStateEvaluator>();
	stateEvaluator_->SetBossEnemy(this);
	stateEvaluator_->Initialize();

	// エフェクトの初期化
	effect_ = std::make_unique<BossEffect>();
	effect_->SetBossEnemy(this);
	effect_->Init();

	// ステートの初期化
	ChangeState(std::make_unique<BossIdleState>(this));
}

void BossEnemy::Update()
{
#ifdef ENABLE_EDITOR
	items_->Editor();
	stateEvaluator_->DrawImGui();
#endif // ENABLE_EDITOR

	// ボスをスタートさせる
	uint32_t clearKill = static_cast<uint32_t>(player_->GetItem()->GetPlayerData().clearKill);
	if (state_->GetState() == BossState::Idle && spawnerFactory_->GetKnockdownCount() >= clearKill) {
		StartBossEnemy();
	}

	// レイの更新
	const float attackIn = items_->GetMainData().rayDistance;
	Vector3 direction = Vector3(player_->GetTransform().translation_ - transform_.translation_).Normalize();
	ray_->Update(transform_.translation_ + items_->GetMainData().rayOffset, direction * attackIn);

	// ステートの更新
	state_->Update();

	// エフェクトの更新
	effect_->Update();

	// 敵コライダーの更新
	const auto& data = items_->GetMainData();
	Collider::radius_ = data.colliderSize;
	Collider::centerPosition_ = transform_.translation_ + data.colliderOffset;
	Collider::Update();
	attackCollider_->Update();
	// アニメーションの更新
	if (Animation::GetMaterial().enableDraw) {
		Animation::SetTransformScale(Vector3::ExprUnitXYZ * data.modelScale);
		Animation::Update();
	} else {
		Animation::TransformUpdate();
	}
}

void BossEnemy::Draw()
{
	effect_->Draw();
}

void BossEnemy::OnCollisionEnter(Collider* other)
{
	if (state_->GetState() == BossState::Dead) { return; }

	bool isPlayer = other->GetColliderName() == "Player";
	bool isCompanion = other->GetColliderName() == "MuscleCompanion";
	bool isSearchDashCompanion = other->GetColliderName() == "SearchDashMuscleCompanion";
	bool isCompanionAttack = other->GetColliderName() == "MuscleCompanionAttack";
	bool isBlowDashCompanion = other->GetColliderName() == "BlowDashMuscleCompanion";

	if (isCompanion || isCompanionAttack || isSearchDashCompanion || isBlowDashCompanion) {
		// 小さな当たり判定は無視する
		if (other->GetRadius() < 0.6f) {
			return;
		}
		// ダメージ処理
		if (isBlowDashCompanion) {
			if (currentHp_ > 2) {
				currentHp_ -= 3;
			} else {
				currentHp_ = 0;
			}
		} else {
			--currentHp_;
		}
		if (isSearchDashCompanion) {
			effect_->OnceHitExplosionBlueEffect();
		} else {
			effect_->OnceHitExplosionEffect();
		}
		// 今のHPが0になったら死亡ステートになる
		currentHp_ = std::clamp(currentHp_, 0u, 10000u);
		if (currentHp_ <= 0) {
			ChangeState(std::make_unique<BossDeadState>(this));
		}
	}

	// ダッシュ攻撃中に建物に当たったら移動に戻る
	if (state_->GetState() == BossState::DashAttack) {
		if (CollisionFilter::CheckColliderNameFieldObject(other->GetColliderName())) {
			ChangeState(std::make_unique<BossDownState>(this));
		}
	}

}

void BossEnemy::OnCollisionStay(Collider* other)
{
	if (state_->GetState() == BossState::Dead) { return; }

	bool isPlayer = other->GetColliderName() == "Player";
	bool isCompanion = other->GetColliderName() == "MuscleCompanion";
	bool isCompanionAttack = other->GetColliderName() == "MuscleCompanionAttack";

	// 移動中にプレイヤーか仲間に触れたら近接範囲攻撃
	if (state_->GetState() == BossState::Move) {
		if (isPlayer || isCompanion || isCompanionAttack) {
			ChangeState(std::make_unique<BossMeleeState>(this));
		}
	}

	// 建物系の押し出し判定(OBB,Sphere)、木の押し出し判定(OBB)
	if (CollisionFilter::CheckColliderNameFieldObject(other->GetColliderName()) &&
		state_->GetState() == BossState::DashAttack) {
		Vector3 push{};
		if (other->GetMyColliderType() == ColliderType::kOBB) {
			push = Collision3D::GetOBBSpherePushVector(other, this);
		} else if (other->GetMyColliderType() == ColliderType::kSphere) {
			push = Collision3D::GetSphereSpherePushVector(other, this);
		}
		push.y = 0.0f;
		transform_.translation_ += push * 100.0f * DeltaTimer::GetDeltaTime();
		Collider::centerPosition_ = transform_.translation_;
		Collider::Update();
		Animation::TransformUpdate();
	}
}

void BossEnemy::OnCollisionExit(Collider* other)
{
}

void BossEnemy::ChangeState(std::unique_ptr<BossBaseState> newState)
{
	if (state_ != nullptr) {
		state_->Finalize();
	}
	state_ = std::move(newState);
	state_->Init();
}

void BossEnemy::ResetSearch(const Vector3& goalPosition)
{
	pathFinder_.Search(transform_.translation_, goalPosition);
}

void BossEnemy::StartBossEnemy()
{
	Animation::PlayByName("Idle", 0.0f);
	Animation::GetMaterial().enableDraw = true;
	Animation::GetMaterial().outlineMask = true;
	Collider::isActive_ = true;
	ray_->SetActive(true);
	ChangeState(std::make_unique<BossAppearState>(this));

	// HPの初期化
	maxHp_ = items_->GetMainData().maxHP;
	currentHp_ = maxHp_;

	// スコアの初期化
	stateEvaluator_->ScoreReset();
}

void BossEnemy::Reset()
{
	// アニメーションを設定
	Animation::PlayByName("Idle", 0.0f);
	Animation::GetMaterial().enableDraw = false;
	Animation::GetMaterial().outlineMask = false;
	Animation::SetTransformTranslation(items_->GetMainData().startPosition);
	Animation::TransformUpdate();

	// コライダーを設定
	Collider::isActive_ = false;
	Collider::Update();

	// ステートの初期化
	ChangeState(std::make_unique<BossIdleState>(this));
}
