#include "RangedEliteEnemy.h"

#include "Objects/Enemy/AdjustItem/EnemyAdjustItem.h"
#include "Objects/Enemy/State/EnemyMoveState.h"

void RangedEliteEnemy::Initialize()
{
	// タイプを設定
	type_ = EnemyType::kRangedElite;

	// 敵Animationの初期化
	Animation::Initialize("Characters_Shaun.gltf");
	Animation::PlayByName("Idle");

	// 弾を3つ作成
	for (uint32_t i = 0; i < 3; ++i) {
		std::unique_ptr<EnemyBullet> bullet = std::make_unique<EnemyBullet>();
		bullet->SetItem(items_);
		bullet->Init("EnemyRangedElite", type_);
		bullets_.push_back(std::move(bullet));
	}

	// 基底クラスの初期化
	BaseEnemy::Initialize();

	// 体力の初期化
	maxHp_ = items_->GetRangedEliteData().tempData.maxHp;
	currentHp_ = maxHp_;
}

void RangedEliteEnemy::Update()
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

void RangedEliteEnemy::Draw()
{
	effect_->Draw();
}

void RangedEliteEnemy::Dead()
{
	// 基底クラスの死亡処理
	BaseEnemy::Dead();
}

void RangedEliteEnemy::Reset(const Vector3& position)
{
	// 基底クラスのリセット処理
	BaseEnemy::Reset(position);
	// データを取得する
	const auto& data = items_->GetRangedEliteData().tempData;
	// 体力を最大値に戻す
	maxHp_ = data.maxHp;
	currentHp_ = maxHp_;
	// Animationの再生を初期化
	transform_.scale_ = Vector3::ExprUnitXYZ * data.modelScale;
	Animation::ForcePlayByName("Run");
	// ステートを初期化
	ChangeState(std::make_unique<EnemyMoveState>(this));
}
