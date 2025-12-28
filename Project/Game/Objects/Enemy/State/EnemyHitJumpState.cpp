#include "EnemyHitJumpState.h"

#include "DeltaTimer.h"

#include "Objects/Enemy/Base/BaseEnemy.h"
#include "Objects/Enemy/AdjustItem/EnemyAdjustItem.h"
#include "Objects/Enemy/State/EnemyMoveState.h"

EnemyHitJumpState::EnemyHitJumpState(BaseEnemy* enemy) :EnemyBaseState(enemy) {}

void EnemyHitJumpState::Init()
{
	// ジャンプ関連パラメータを取得
	const auto data = enemy_->GetItem()->GetMainData();
	hitJumpVelocityY_ = data.hitJumpVelocityY;
	hitJumpAccelerY_ = data.hitJumpAccelerY;
	hitJumpNockbackScale_ = data.hitJumpNockbackScale;
	
	// 開始Y座標を初期化
	startY_ = enemy_->GetTransform().translation_.y;
}

void EnemyHitJumpState::Finalize()
{
}

void EnemyHitJumpState::Update()
{
    // 速度更新（加速度）
    hitJumpVelocityY_ += hitJumpAccelerY_ * DeltaTimer::GetDeltaTime();

    // 位置更新（Y方向）
    float translateY = enemy_->GetTransform().translation_.y;
    translateY += hitJumpVelocityY_ * DeltaTimer::GetDeltaTime();

	// ノックバック方向の計算
	Vector3 veloctiy = Vector3::ExprUnitZ.Transform(Quaternion::MakeRotateMatrix(enemy_->GetTransform().rotation_));
	veloctiy = veloctiy.Normalize() * hitJumpNockbackScale_ * DeltaTimer::GetDeltaTime();

    // 座標を更新する
    SetTranslate(translateY, veloctiy);

    // 着地判定：開始Yより下に行ったら着地
    if (translateY <= startY_)
    {
        translateY = startY_;
        hitJumpVelocityY_ = 0.0f;
        // 座標を更新する
        SetTranslate(translateY, veloctiy);
        if (enemy_->GetHitCollider()) {
            // 攻撃ステートに遷移する
            enemy_->TypeChengeAttackState();
        } else {
			enemy_->ChangeState(std::make_unique<EnemyMoveState>(enemy_));
        }
        return;
    }
}

void EnemyHitJumpState::Draw()
{
}

void EnemyHitJumpState::SetTranslate(float y, const Vector3& velocity)
{
    Vector3 translation = enemy_->GetTransform().translation_ + velocity;
    translation.y = y;
    enemy_->SetTransformTranslation(translation);
}

