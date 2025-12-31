#pragma once
#include <memory>

#include "BaseScene.h"

#include "Object3d.h"
#include "Animation.h"
#include "PrimitiveDrawr.h"
#include "ParticleEmitter.h"

#include "scene/sceneFade/SceneFade.h"

#include "Objects/Player/Player.h"
#include "Objects/Enemy/Spawner/EnemySpawnerFactory.h"
#include "Objects/FieldObject/Factory/FieldObjectFactory.h"
#include "Objects/MuscleCompanion/Manager/MuscleCompanionManager.h"
#include "Objects/Boss/Base/BossEnemy.h"

#include "GameCamera/GameCamera.h"
#include "SearchAlgorithm/Collision/MapCollision.h"
#include "Uis/GameSceneUIs/GameSceneUIs.h"

/// <summary>
/// ゲームプレイシーン
/// </summary>
class GamePlayScene : public BaseScene
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

private:

	std::unique_ptr<PrimitiveDrawr> skyBox_ = nullptr;
	
	// プレイヤーの生成
	std::unique_ptr<Player> player_ = nullptr;

	// プレイヤーの仲間管理クラスを生成
	std::unique_ptr<MuscleCompanionManager> companionManager_ = nullptr;

	// 敵の生成
	std::unique_ptr<EnemySpawnerFactory> enemySpawnerFactory_ = nullptr;

	// ボス敵の生成
	std::unique_ptr<BossEnemy> bossEnemy_ = nullptr;

	// フィールド場のオブジェクト
	std::unique_ptr<FieldObjectFactory> fieldObjectFactory_ = nullptr;

	// ゲームカメラの生成
	std::unique_ptr<GameCamera> gameCamera_ = nullptr;

	// 探索アルゴリズムの判定
	std::unique_ptr<MapCollision> mapCollision_ = nullptr;

	// ゲームの操作UI
	std::unique_ptr<GameSceneUIs> gameSceneUis_ = nullptr;

	bool isSelect_ = false;
};

