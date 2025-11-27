#include "MeleeEnemy.h"

#include "DeltaTimer.h"

#include "objects/enemy/state/EnemyMoveState.h"
#include "objects/enemy/weapon/axe/EnemyAxe.h"

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

	// Animationの再生を初期化
	Animation::PlayByName("Run_Arms");
	// ステートを初期化
	ChengeState(std::make_unique<EnemyMoveState>(this));
}