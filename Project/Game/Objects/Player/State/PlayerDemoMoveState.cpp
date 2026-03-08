#include "PlayerDemoMoveState.h"

#include "SceneJsonLoader.h"
#include "DeltaTimer.h"

#include "Objects/Player/Player.h"

PlayerDemoMoveState::PlayerDemoMoveState(Player* player) :PlayerBaseState(player) {}

void PlayerDemoMoveState::Init()
{
	// アニメーションを再生する
	player_->PlayByName("Run");

	// シーンのオブジェクトをロード、初期化
	const auto loader = player_->GetSceneJsonLoader();
	for (auto it = loader->GetData().begin(); it != loader->GetData().end();) {
		if (it->second.tag == "DemoPoint") {
			checkPoints_.push_back(it->second.transform.translation_);
		}
		++it;
	}

	// 探索を初期化する
	InitSearch();
}

void PlayerDemoMoveState::Finalize()
{
	auto& pathFinder = player_->GetPathFinder();
	pathFinder.DebugSpline(false);
}

void PlayerDemoMoveState::Update()
{
	// 方向を計算
	const float speed = player_->GetItem()->GetPlayerData().speed;
	auto& pathFinder = player_->GetPathFinder();

	// 探索の更新
	ResetSearch();
	pathFinder.Update(speed);
	pathFinder.DebugSpline(true);
	Vector3 velocity = pathFinder.GetVelocity();
	velocity.y = 0.0f;
	if (velocity.Length() != 0.0f) { velocity = velocity.Normalize(); }

	// 回転処理
	if (velocity.Length() != 0.0f) {
		Quaternion yRotation = pathFinder.GetRotation();
		yRotation = Quaternion::Slerp(player_->GetTransform().rotation_, yRotation, 0.2f);
		player_->SetTransformRotation(yRotation);
	}
	// 移動処理
	Vector3 position = player_->GetTransform().translation_;
	player_->SetTransformTranslation(position + velocity * speed * DeltaTimer::GetDeltaTime());
}

void PlayerDemoMoveState::Draw()
{
}

void PlayerDemoMoveState::InitSearch()
{
	// ランダムで次の地点を決める
	std::mt19937 randomEngine(seedGenerator_());
	std::uniform_int_distribution<size_t> MaxSize(1, checkPoints_.size());
	size_t randIndex = MaxSize(randomEngine) - 1;
	nowCheckPoint_ = checkPoints_[randIndex];

	// チェックポイントを変更する
	auto& pathFinder = player_->GetPathFinder();
	pathFinder.Search(player_->GetTransform().translation_, nowCheckPoint_);
}

void PlayerDemoMoveState::ResetSearch()
{
	// チェックポイントに到達していなければ即リターン
	float distance = Vector3::Distance(nowCheckPoint_, player_->GetTransform().translation_);
	if (distance > player_->GetItem()->GetPlayerData().checkPointDistance) { return; }

	// 探索を初期化する
	InitSearch();
}