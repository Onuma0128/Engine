#pragma once
#include <memory>

#include "BaseScene.h"

#include "Object3d.h"
#include "Animation.h"
#include "PrimitiveDrawr.h"
#include "ParticleEmitter.h"

#include "gameScene/player/Player.h"
#include "gameScene/enemy/spawner/EnemySpawnerFactory.h"
#include "gameScene/gameCamera/GameCamera.h"
#include "gameScene/fieldObject/Factory/FieldObjectFactory.h"


class GamePlayScene : public BaseScene
{
public:

	void Initialize() override;

	void Finalize() override;

	void Update() override;

	void Draw() override;

private:

	std::unique_ptr<PrimitiveDrawr> skyBox_ = nullptr;
	std::shared_ptr<ParticleEmitter> emitter_ = nullptr;
	
	// プレイヤーの生成
	std::unique_ptr<Player> player_ = nullptr;

	// 敵の生成
	std::unique_ptr<EnemySpawnerFactory> enemySpawnerFactory_ = nullptr;

	// フィールド場のオブジェクト
	std::unique_ptr<FieldObjectFactory> fieldObjectFactory_ = nullptr;

	// ゲームカメラの生成
	std::unique_ptr<GameCamera> gameCamera_ = nullptr;
};

