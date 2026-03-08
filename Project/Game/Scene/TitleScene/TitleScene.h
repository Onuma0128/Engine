#pragma once
#include <memory>

#include "BaseScene.h"
#include "ParticleManager.h"
#include "ParticleEmitter.h"
#include "Animation.h"
#include "Camera.h"
#include "Audio.h"

#include "Objects/Player/Player.h"
#include "Objects/Enemy/Spawner/EnemySpawnerFactory.h"
#include "Objects/FieldObject/Factory/FieldObjectFactory.h"
#include "Objects/MuscleCompanion/Manager/MuscleCompanionManager.h"

#include "GameCamera/GameCamera.h"
#include "SearchAlgorithm/Collision/MapCollision.h"
#include "Scene/SceneFade/SceneFade.h"
#include "Uis/TitleUI/TitleUI.h"

/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene : public BaseScene
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

	// フェード
	std::unique_ptr<BaseUI> sceneFade_ = nullptr;
	bool isFade_ = false;

	// タイトル用UI
	std::unique_ptr<TitleUI> titleUI_ = nullptr;

	// プレイヤーの生成
	std::unique_ptr<Player> player_ = nullptr;

	// プレイヤーの仲間管理クラスを生成
	std::unique_ptr<MuscleCompanionManager> companionManager_ = nullptr;

	// 敵の生成
	std::unique_ptr<EnemySpawnerFactory> enemySpawnerFactory_ = nullptr;

	// フィールド場のオブジェクト
	std::unique_ptr<FieldObjectFactory> fieldObjectFactory_ = nullptr;

	// ゲームカメラの生成
	std::unique_ptr<GameCamera> gameCamera_ = nullptr;

	// 探索アルゴリズムの判定
	std::unique_ptr<MapCollision> mapCollision_ = nullptr;


	std::unique_ptr<Audio> bgm_;

};