#include "MeleeEnemy.h"

#include "DeltaTimer.h"

#include "Objects/Enemy/AdjustItem/EnemyAdjustItem.h"
#include "Objects/Enemy/State/EnemyMoveState.h"
#include "Objects/Enemy/Weapon/Axe/EnemyAxe.h"

void MeleeEnemy::Initialize()
{
	// タイプを設定
	type_ = EnemyType::kMelee;

	// 敵Animationの初期化
	Animation::Initialize("Zombie_Basic.gltf");
	Animation::PlayByName("Idle");

	// 近接攻撃用のコライダーを作成
	weapon_ = std::make_unique<EnemyAxe>(this);
	weapon_->Init(ColliderType::kSphere, "EnemyMelee");

	// 基底クラスの初期化
	BaseEnemy::Initialize();

	// 体力の初期化
	maxHp_ = items_->GetMeleeData().tempData.maxHp;
	currentHp_ = maxHp_;
}

void MeleeEnemy::Update()
{
	// 基底クラスの更新
	BaseEnemy::Update();

	// ウエポンの更新
	weapon_->Update();
}

void MeleeEnemy::Draw()
{
	effect_->Draw();
}

void MeleeEnemy::Dead()
{
	// 基底クラスの死亡処理
	BaseEnemy::Dead();
}

void MeleeEnemy::Reset(const Vector3& position)
{
	// 基底クラスのリセット処理
	BaseEnemy::Reset(position);
	// データを取得する
	const auto& data = items_->GetMeleeData().tempData;
	// 体力を最大値に戻す
	maxHp_ = data.maxHp;
	 currentHp_ = maxHp_;
	// Animationの再生を初期化
	transform_.scale_ = Vector3::ExprUnitXYZ * data.modelScale;
	Animation::ForcePlayByName("Run_Arms");
	// ステートを初期化
	ChangeState(std::make_unique<EnemyMoveState>(this));
}