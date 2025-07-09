#include "ShieldBearerEnemy.h"

#include "DeltaTimer.h"

#include "../state/EnemyMoveState.h"
#include "../weapon/axe/EnemyAxe.h"
#include "../weapon/shield/EnemyShield.h"

void ShieldBearerEnemy::Initialize()
{
	// タイプを設定
	type_ = EnemyType::ShieldBearer;

	// 敵Animationの初期化
	Animation::Initialize("Zombie_Basic.gltf");
	Animation::PlayByName("Idle");
	Animation::SetSceneRenderer();
	Animation::GetMaterial().enableDraw = false;
	Animation::SetDrawBone(false);
	Animation::GetTimeStop() = true;
	Animation::transform_.scale_ *= 1.5f;

	// シールド用のコライダーを作成
	shieldWeapon_ = std::make_unique<EnemyShield>(this);
	shieldWeapon_->Init(ColliderType::OBB, "EnemyShield");
	shieldWeapon_->SetIsActive(false);
	// 攻撃コライダーを作成
	weapon_ = std::make_unique<EnemyAxe>(this);
	weapon_->Init(ColliderType::Sphere, "EnemyShieldBearer");

	// 基底クラスの初期化
	BaseEnemy::Initialize();
}

void ShieldBearerEnemy::Update()
{
	// 基底クラスの更新
	BaseEnemy::Update();

	// ウエポンの更新
	weapon_->Update();
	shieldWeapon_->Update();
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
	ChengeState(std::make_unique<EnemyMoveState>(this));
}
