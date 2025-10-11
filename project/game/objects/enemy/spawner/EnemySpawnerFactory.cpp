#include "EnemySpawnerFactory.h"

#include "DeltaTimer.h"

#include "objects/enemy/base/MeleeEnemy.h"
#include "objects/enemy/base/RangedEnemy.h"
#include "objects/enemy/base/ShieldBearerEnemy.h"
#include "objects/enemy/base/RangedEliteEnemy.h"

#include "objects/player/Player.h"
#include "gameCamera/GameCamera.h"

void EnemySpawnerFactory::Init(SceneJsonLoader loader)
{
	items_ = std::make_unique<EnemyAdjustItem>();
	items_->LoadItems();
	
	for (auto it = loader.GetData().begin(); it != loader.GetData().end();) {
		if (it->second.groupName == "EnemySpawner") {
			CreateSpawner(it->second);
		}
		++it;
	}

	// Release用
	size_t normal = 20;
	size_t strong = 10;
	// Debug用
#ifdef _DEBUG
	normal = 15;
	strong = 5;
#endif // _DEBUG

	// 敵をタイプごとに初期化
	InitTypeEnemy(EnemyType::Melee, enemyMelees_, normal);
	InitTypeEnemy(EnemyType::Ranged, enemyRnageds_, normal);
	InitTypeEnemy(EnemyType::ShieldBearer, enemyShieldBearers_, strong);
	InitTypeEnemy(EnemyType::RangedElite, enemyRnagedElites_, strong);

	spawnInterval_ = 1.0f;
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
	spawner->Init(object);
	enemySpawners_.push_back(std::move(spawner));
}

void EnemySpawnerFactory::InitTypeEnemy(EnemyType type, std::list<std::unique_ptr<BaseEnemy>>& enemys, size_t size)
{
	enemys.resize(size);

	for (auto& enemy : enemys) {
		switch (type) {
		case EnemyType::Melee:			enemy = std::make_unique<MeleeEnemy>(); break;
		case EnemyType::Ranged:			enemy = std::make_unique<RangedEnemy>(); break;
		case EnemyType::ShieldBearer:	enemy = std::make_unique<ShieldBearerEnemy>(); break;
		case EnemyType::RangedElite:	enemy = std::make_unique<RangedEliteEnemy>(); break;
		default:break;
		}
		enemy->SetPlayer(player_);
		enemy->SetGameCamera(gameCamera_);
		enemy->SetItem(items_.get());
		enemy->SetMapData(mapData_);
		enemy->Initialize();
	}
}

void EnemySpawnerFactory::UpdateTypeEnemy(std::list<std::unique_ptr<BaseEnemy>>& enemys)
{
	for (auto& enemy : enemys) {
		enemy->TransformUpdate();
	}
}

void EnemySpawnerFactory::RandomSpawnEnemy()
{
#ifdef _DEBUG
	if (items_->GetMainData().nowSpawn) {
		// タイプごとに敵を生成
		EnemyType type = static_cast<EnemyType>(items_->GetMainData().spawnIndex);
		switch (type) {
		case EnemyType::Melee:			ResetTypeEnemy(enemyMelees_, enemySpawners_[0]); break;
		case EnemyType::Ranged:			ResetTypeEnemy(enemyRnageds_, enemySpawners_[0]); break;
		case EnemyType::ShieldBearer:	ResetTypeEnemy(enemyShieldBearers_, enemySpawners_[0]); break;
		case EnemyType::RangedElite:	ResetTypeEnemy(enemyRnagedElites_, enemySpawners_[0]); break;
		default: break;
		}
	}
	if (!items_->GetMainData().debugIsSpawn) { return; }
#endif // _DEBUG

	// 湧き上限を超えていないかチェックする
	size_t enemySpawnCount = 0;
	uint32_t kNockdownCount = 0;
	for (auto& spawner : enemySpawners_) {
		enemySpawnCount += spawner->GetEnemyList().size();
		kNockdownCount += spawner->GetNockdownCount();
	}
	if (enemySpawnCount >= items_->GetMainData().maxSpawn) { return; }

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
		if (kNockdownCount <= static_cast<uint32_t>(items_->GetMainData().nextWaveKillCount)) {
			std::uniform_int_distribution<int> interval(1, 5);
			spawnInterval_ = static_cast<float>(interval(randomEngine_)) * 0.8f;
		} else {
			std::uniform_int_distribution<int> interval(1, 5);
			spawnInterval_ = static_cast<float>(interval(randomEngine_)) * 0.5f;
		}
		spawnTime_ = 0.0f;
	}
}

void EnemySpawnerFactory::ResetTypeEnemy(std::list<std::unique_ptr<BaseEnemy>>& enemys, std::unique_ptr<EnemySpawner>& spawner)
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
