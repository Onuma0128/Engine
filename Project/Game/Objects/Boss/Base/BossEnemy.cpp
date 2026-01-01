#include "BossEnemy.h"

#include "Collision/CollisionFilter.h"

#include "Objects/Boss/State/BossMoveState.h"
#include "Objects/Boss/State/BossDownState.h"
#include "Objects/Boss/State/BossMeleeState.h"
#include "Objects/Boss/State/BossDeadState.h"

#include "Objects/Player/Player.h"

void BossEnemy::Initialize()
{
	// 調整項目の設定
	items_ = std::make_unique<BossAdjustItem>();
	items_->LoadItems();

	// アニメーションを設定
	Animation::Initialize("BossEnemy.gltf");
	Animation::SetSceneRenderer();
	Animation::PlayByName("Idle", 0.0f);
	Animation::GetMaterial().outlineMask = true;
	Animation::GetMaterial().outlineColor = Vector3::ExprZero;
	Animation::SetTransformTranslation(items_->GetMainData().startPosition);

	// コライダーを設定
	Collider::AddCollider();
	Collider::colliderName_ = "BossEnemy";
	Collider::myType_ = ColliderType::kSphere;
	Collider::targetColliderName_ = { 
		"Player","MuscleCompanion","PlayerShotRay","MuscleCompanionAttack",
		"Building","DeadTree","fence","Bush","StoneWall","ShortStoneWall",
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
	ray_->SetActive(true);

	stateEvaluator_ = std::make_unique<BossStateEvaluator>();
	stateEvaluator_->SetBossEnemy(this);
	stateEvaluator_->Initialize();

	ChangeState(std::make_unique<BossMoveState>(this));
}

void BossEnemy::Update()
{
#ifdef ENABLE_EDITOR
	items_->Editor();
#endif // ENABLE_EDITOR

	stateEvaluator_->DrawImGui();

	// レイの更新
	const float attackIn = items_->GetMainData().rayDistance;
	Vector3 direction = Vector3(player_->GetTransform().translation_ - transform_.translation_).Normalize();
	ray_->Update(transform_.translation_ + items_->GetMainData().rayOffset, direction * attackIn);

	// ステートの更新
	state_->Update();

	// 敵コライダーの更新
	const auto& data = items_->GetMainData();
	Collider::radius_ = data.colliderSize;
	Collider::centerPosition_ = transform_.translation_ + data.colliderOffset;
	Collider::Update();
	attackCollider_->Update();
	// アニメーションの更新
	Animation::Update();
}

void BossEnemy::Draw()
{
}

void BossEnemy::OnCollisionEnter(Collider* other)
{
	if (state_->GetState() == BossState::Dead) { return; }

	bool isPlayer = other->GetColliderName() == "Player";
	bool isCompanion = other->GetColliderName() == "MuscleCompanion";
	bool isCompanionAttack = other->GetColliderName() == "MuscleCompanionAttack";

	if (isCompanion || isCompanionAttack) {
		// 小さな当たり判定は無視する
		if (other->GetRadius() < 0.5f) {
			return;
		}
		// ダメージ処理
		--currentHp_;
		// 今のHPが0になったら死亡ステートになる
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

