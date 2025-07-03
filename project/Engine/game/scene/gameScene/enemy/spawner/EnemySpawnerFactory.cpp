#include "EnemySpawnerFactory.h"

#include <thread>

#include "DeltaTimer.h"

#include "gameScene/player/Player.h"
#include "gameScene/gameCamera/GameCamera.h"

void EnemySpawnerFactory::Init(SceneJsonLoader loader)
{
	items_ = std::make_unique<EnemyAdjustItem>();
	items_->LoadItems();
	
	for (auto it = loader.GetData().begin(); it != loader.GetData().end();) {
		if (it->second.tag == "EnemySpawner") {
			CreateSpawner(it->second);
		}
		++it;
	}

	// 敵をタイプごとに初期化
	InitTypeEnemy(EnemyType::Melee, enemyMelees_, 1);
	InitTypeEnemy(EnemyType::Ranged, enemyRnageds_, 1);
	InitTypeEnemy(EnemyType::ShieldBearer, enemyShieldBearers_, 0);
	InitTypeEnemy(EnemyType::RangedElite, enemyRnagedElites_, 0);

	spawnInterval_ = 10.0f;
}

void EnemySpawnerFactory::Update()
{
	items_->Editor();

	// 敵のランダムスポーン
	RandomSpawnEnemy();

	// スポナーの更新
	uint32_t kNockdownCount = 0;
	for (auto& spawner : enemySpawners_) {
		spawner->Update();
		kNockdownCount += spawner->GetNockdownCount();
	}

	// スポーンしていない敵の更新処理
	UpdateTypeEnemy(enemyMelees_);
	UpdateTypeEnemy(enemyRnageds_);
	UpdateTypeEnemy(enemyShieldBearers_);
	UpdateTypeEnemy(enemyRnagedElites_);
}

void EnemySpawnerFactory::Draw()
{
	for (auto& spawner : enemySpawners_) {
		spawner->Draw();
	}

}

void EnemySpawnerFactory::CreateSpawner(SceneObject object)
{
	std::unique_ptr<EnemySpawner> spawner = std::make_unique<EnemySpawner>();
	spawner->SetPlayer(player_);
	spawner->SetGameCamera(gameCamera_);
	spawner->SetItem(items_.get());
	spawner->Init(object);
	enemySpawners_.push_back(std::move(spawner));
}

void EnemySpawnerFactory::InitTypeEnemy(EnemyType type, std::list<std::unique_ptr<Enemy>>& enemys, size_t size)
{
	enemys.resize(size);

	for (auto& enemy : enemys) {
		enemy = std::make_unique<Enemy>();
		enemy->SetPlayer(player_);
		enemy->SetGameCamera(gameCamera_);
		enemy->SetItem(items_.get());
		enemy->Init(type);
	}
}

void EnemySpawnerFactory::UpdateTypeEnemy(std::list<std::unique_ptr<Enemy>>& enemys)
{
	for (auto& enemy : enemys) {
		enemy->TransformUpdate();
	}
}

void EnemySpawnerFactory::RandomSpawnEnemy()
{
	// 湧き上限を超えていないかチェックする
	size_t enemySpawnCount = 0;
	uint32_t kNockdownCount = 0;
	for (auto& spawner : enemySpawners_) {
		enemySpawnCount += spawner->GetEnemyList().size();
		kNockdownCount += spawner->GetNockdownCount();
	}
	if (enemySpawnCount >= 20) { return; }

	// スポーンタイムを更新
	spawnTime_ += DeltaTimer::GetDeltaTime();
	// 敵をスポーンさせる
	if (spawnTime_ > spawnInterval_) {
		// ランダムで敵のタイプを決める
		std::mt19937 randomEngine_(seedGenerator_());
		// 何処のスポナーに湧くか
		std::uniform_int_distribution<int> spawner(0, static_cast<int>(enemySpawners_.size() - 1));
		int spownNumber = (spawner(randomEngine_));
		// 何の敵が湧くか
		EnemyType type = EnemyType::Melee;
		// 倒した数が25を超えていないなら
		if (kNockdownCount <= 25) {
			std::uniform_int_distribution<int> enemyType(0, 1);
			type = static_cast<EnemyType>(enemyType(randomEngine_));
		} else {
			std::uniform_int_distribution<int> enemyType(0, 5);
			int randomType = (enemyType(randomEngine_)) % 4;
			type = static_cast<EnemyType>(randomType);
		}
		// タイプごとに敵を生成
		switch (type) {
		case EnemyType::Melee:			ResetTypeEnemy(enemyMelees_, enemySpawners_[spownNumber]); break;
		case EnemyType::Ranged:			ResetTypeEnemy(enemyRnageds_, enemySpawners_[spownNumber]); break;
		case EnemyType::ShieldBearer:	ResetTypeEnemy(enemyShieldBearers_, enemySpawners_[spownNumber]); break;
		case EnemyType::RangedElite:	ResetTypeEnemy(enemyRnagedElites_, enemySpawners_[spownNumber]); break;
		default: break;
		}

		// スポーン時間を初期化
		if (kNockdownCount <= 25) {
			std::uniform_int_distribution<int> interval(1, 5);
			spawnInterval_ = static_cast<float>(interval(randomEngine_)) * 0.8f;
		} else {
			std::uniform_int_distribution<int> interval(1, 5);
			spawnInterval_ = static_cast<float>(interval(randomEngine_)) * 0.5f;
		}
		spawnTime_ = 0.0f;
	}
}

void EnemySpawnerFactory::ResetTypeEnemy(std::list<std::unique_ptr<Enemy>>& enemys, std::unique_ptr<EnemySpawner>& spawner)
{
	for (auto& enemy : enemys) {
		if (!enemy->GetEnableMove()) {
			Vector3 pos = spawner->GetTransform().translation_;
			enemy->Reset(Vector3{ pos.x,0.0f,pos.z });
			spawner->GetEnemyList().push_back(enemy.get());
			break;
		}
	}
}
