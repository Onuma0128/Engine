#include "MuscleCompanion.h"

#include "DeltaTimer.h"

#include "Collision/CollisionFilter.h"
#include "Objects/Player/Player.h"
#include "SearchAlgorithm/Collision/MapCollision.h"

#include "Objects/MuscleCompanion/State/CompanionIdleState.h"
#include "Objects/MuscleCompanion/State/CompanionMoveState.h"
#include "Objects/MuscleCompanion/State/CompanionAttackState.h"

void MuscleCompanion::Initialize()
{
	// 基本的なアニメーションを設定
	Animation::Initialize("muscleCompanion.gltf");
	Animation::SetSceneRenderer();
	Animation::PlayByName("Idle");
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
		"MuscleCompanion","Enemy","EnemyRay","EnemyMelee",
		"Building","DeadTree","fence","Bush","StoneWall","ShortStoneWall",
	};
	Collider::DrawCollider();

	// ステートの初期化
	ChangeState(std::make_unique<CompanionMoveState>(this));

	attackCollider_ = std::make_unique<CompanionAttackCollider>();
	attackCollider_->SetCompanion(this);
	attackCollider_->Initialize();

	// ダッシュ時の一回目の攻撃フラグの初期化
	isFirstDashAttack_ = true;
	// 集合要求フラグの初期化
	isGatherRequested_ = true;
	// HPの初期化
	maxHp_ = items_->GetMainData().maxHP;
	currentHp_ = maxHp_;
}

void MuscleCompanion::Update()
{
	// ステートの更新
	state_->Update();


	// コライダーの更新
	attackCollider_->Update();
	Collider::radius_ = items_->GetMainData().colliderSize * dashColliderScale_;
	Collider::centerPosition_ = transform_.translation_ + items_->GetMainData().colliderOffset;
	Collider::Update();

	// アニメーションの更新
	Animation::Update();
}

void MuscleCompanion::Draw()
{
	state_->Draw();
}

void MuscleCompanion::OnCollisionEnter(Collider* other)
{
	// ダッシュ状態で当たったら
	if (state_->GetState() == CharacterState::Dash) {
		// 建物に当たったら待機状態へ
		if (CollisionFilter::CheckColliderNameFieldObject(other->GetColliderName())) {
			ChangeState(std::make_unique<CompanionIdleState>(this));
		// 敵に当たったら攻撃状態へ
		} else if (other->GetColliderName() == "Enemy") {
			isFirstDashAttack_ = true;
			Vector3 velocity = other->GetCenterPosition() - transform_.translation_;
			Quaternion yRotation_ = Quaternion::DirectionToQuaternion(
				transform_.rotation_, velocity.Normalize(), 1.0f);
			transform_.rotation_ = yRotation_;
			ChangeState(std::make_unique<CompanionAttackState>(this));
		}
	}
	// 敵の攻撃に当たったら体力を1減らす
	if (CollisionFilter::CheckColliderNameEnemy(other->GetColliderName())) {
		--currentHp_;
	}
}

void MuscleCompanion::OnCollisionStay(Collider* other)
{
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

void MuscleCompanion::ChangeState(std::unique_ptr<CompanionBaseState> newState)
{
	if (state_ != nullptr) {
		state_->Finalize();
	}
	state_ = std::move(newState);
	state_->Init();
}

void MuscleCompanion::ResetSearch() 
{
	pathFinder_.Search(transform_.translation_, player_->GetTransform().translation_);
}

bool MuscleCompanion::SearchDistance()
{
	// 距離が遠ければtrueを返す
	const float dist = Vector3::Distance(transform_.translation_, player_->GetTransform().translation_);
	if (dist > items_->GetMainData().searchCancelDistance) { return true; }
	return false;
}
