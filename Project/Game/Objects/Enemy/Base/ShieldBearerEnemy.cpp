#include "ShieldBearerEnemy.h"

#include "DeltaTimer.h"

#include "objects/enemy/state/EnemyMoveState.h"
#include "objects/enemy/weapon/axe/EnemyAxe.h"
#include "objects/enemy/weapon/shield/EnemyShield.h"

void ShieldBearerEnemy::Initialize()
{
	// タイプを設定
	type_ = EnemyType::kShieldBearer;

	// 敵Animationの初期化
	Animation::Initialize("Zombie_Basic.gltf");
	Animation::PlayByName("Idle");

	// シールド用のコライダーを作成
	shieldWeapon_ = std::make_unique<EnemyShield>(this);
	shieldWeapon_->Init(ColliderType::kOBB, "EnemyShield");
	shieldWeapon_->SetIsActive(false);
	// 攻撃コライダーを作成
	weapon_ = std::make_unique<EnemyAxe>(this);
	weapon_->Init(ColliderType::kSphere, "EnemyShieldBearer");

	// 基底クラスの初期化
	BaseEnemy::Initialize();
}

void ShieldBearerEnemy::Update()
{
	// 基底クラスの更新
	BaseEnemy::Update();

	// ウエポンの更新
	weapon_->Update();
	if (!stateParam_.isAttacking_) {
		shieldWeapon_->Update();
	} else {
		shieldWeapon_->TransformUpdate();
	}
}

void ShieldBearerEnemy::TransformUpdate()
{
	// 敵の行動許可が出ていなければ更新できない
	BaseEnemy::TransformUpdate();
	if (!stateParam_.enableMove_) {
		shieldWeapon_->Update();
	}
}

void ShieldBearerEnemy::Draw()
{
	// エフェクトの描画
	effect_->Draw();
}

void ShieldBearerEnemy::Dead()
{
	// 基底クラスの死亡処理
	BaseEnemy::Dead();

	// Activeを切る
	shieldWeapon_->SetIsActive(false);
}

void ShieldBearerEnemy::Reset(const Vector3& position)
{
	// 基底クラスのリセット処理
	BaseEnemy::Reset(position);

	// Activeを戻す
	shieldWeapon_->SetIsActive(true);
	// Animationの再生を初期化
	Animation::PlayByName("Run_Arms");
	// ステートを初期化
	ChangeState(std::make_unique<EnemyMoveState>(this));
}
