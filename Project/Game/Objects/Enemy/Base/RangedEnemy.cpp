#include "RangedEnemy.h"

#include "Objects/Enemy/AdjustItem/EnemyAdjustItem.h"
#include "Objects/Enemy/State/EnemyMoveState.h"

void RangedEnemy::Initialize()
{
	// タイプを設定
	type_ = EnemyType::kRanged;

	// 敵Animationの初期化
	Animation::Initialize("Characters_Shaun.gltf");
	Animation::PlayByName("Idle");

	// 弾を1つ作成
	std::unique_ptr<EnemyBullet> bullet = std::make_unique<EnemyBullet>();
	bullet->SetItem(items_);
	bullet->Init("EnemyRanged", type_);
	bullets_.push_back(std::move(bullet));

	// 基底クラスの初期化
	BaseEnemy::Initialize();

	// 体力の初期化
	maxHp_ = items_->GetRangedData().tempData.maxHp;
	currentHp_ = maxHp_;
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
	// データを取得する
	const auto& data = items_->GetRangedData().tempData;
	// 体力を最大値に戻す
	maxHp_ = data.maxHp;
	currentHp_ = maxHp_;
	// Animationの再生を初期化
	transform_.scale_ = Vector3::ExprUnitXYZ * data.modelScale;
	Animation::ForcePlayByName("Run");
	// ステートを初期化
	ChangeState(std::make_unique<EnemyMoveState>(this));
}
