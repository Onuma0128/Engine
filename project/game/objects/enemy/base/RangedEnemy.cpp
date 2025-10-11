#include "RangedEnemy.h"

#include "objects/enemy/state/EnemyMoveState.h"

void RangedEnemy::Initialize()
{
	// タイプを設定
	type_ = EnemyType::Ranged;

	// 敵Animationの初期化
	Animation::Initialize("Characters_Shaun.gltf");
	Animation::PlayByName("Idle");
	Animation::SetSceneRenderer();
	Animation::GetMaterial().enableDraw = false;
	Animation::GetTimeStop() = true;

	// 弾を1つ作成
	std::unique_ptr<EnemyBullet> bullet = std::make_unique<EnemyBullet>();
	bullet->SetItem(items_);
	bullet->Init("EnemyRanged", type_);
	bullets_.push_back(std::move(bullet));

	// 基底クラスの初期化
	BaseEnemy::Initialize();
}

void RangedEnemy::Update()
{
	// 基底クラスの更新
	BaseEnemy::Update();

	// 弾の更新
	for (auto& bullet : bullets_) {
		bullet->Update();
		// 弾が消えた時のコールバック関数
		bullet->SetOnDeactivateCallback([]() {});
	}
}

void RangedEnemy::Draw()
{
	effect_->Draw();
}

void RangedEnemy::Dead()
{
	// 基底クラスの死亡処理
	BaseEnemy::Dead();
}

void RangedEnemy::Reset(const Vector3& position)
{
	// 基底クラスのリセット処理
	BaseEnemy::Reset(position);

	// Animationの再生を初期化
	Animation::PlayByName("Run");
	// ステートを初期化
	ChengeState(std::make_unique<EnemyMoveState>(this));
}
