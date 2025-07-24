#include "GamePlayScene.h"

#include "imgui.h"
#include "ParticleManager.h"
#include "SceneManager.h"
#include "SceneJsonLoader.h"
#include "PostEffectManager.h"

#include "Collision3D.h"
#include "Vector3.h"

void GamePlayScene::Initialize()
{
	// シーンのロード
	SceneJsonLoader loader;
	loader.Load("sceneObject");

	skyBox_ = std::make_unique<PrimitiveDrawr>();
	skyBox_->TypeInit(PrimitiveType::Skybox);
	skyBox_->GetTransform().scale = { 1024.0f,1024.0f ,1024.0f };
	skyBox_->SetSceneRenderer();

	fieldObjectFactory_ = std::make_unique<FieldObjectFactory>();
	fieldObjectFactory_->Init(loader);

	player_ = std::make_unique<Player>();
	player_->Init(loader);

	gameCamera_ = std::make_unique<GameCamera>();
	gameCamera_->SetPlayer(player_.get());
	gameCamera_->Init();

	enemySpawnerFactory_ = std::make_unique<EnemySpawnerFactory>();
	enemySpawnerFactory_->SetPlayer(player_.get());
	enemySpawnerFactory_->SetGameCamera(gameCamera_.get());
	enemySpawnerFactory_->Init(loader);

	mapCollision_ = std::make_unique<MapCollision>();
	mapCollision_->Init(loader);

	sceneFade_ = std::make_unique<SceneFade>();
	sceneFade_->Init();
	sceneFade_->FadeIn(3.0f);

	/*PathFinder pathFinder;
	pathFinder.SetMapData(mapCollision_->GetMapData());
	pathFinder.Search(Vector3{ 0.0f,0.0f,45.0f }, player_->GetTransform().translation_, positions);

	time = 0.0f;
	count = 0;
	test_ = std::make_unique<Object3d>();
	test_->Initialize("Box.obj");
	test_->SetSceneRenderer();
	test_->GetTransform().translation_ = positions[count];*/
}

void GamePlayScene::Finalize()
{
}

void GamePlayScene::Update()
{
	mapCollision_->Update();

	/*ImGui::Begin("A_star");
	ImGui::DragFloat2("startPos", &startPos.x);
	if (ImGui::Button("start")) {
		time = 0.0f;
		count = 0;
		positions.clear();
		PathFinder pathFinder;
		pathFinder.SetMapData(mapCollision_->GetMapData());
		pathFinder.Search(Vector3{ startPos.x,0.0f,startPos.y }, player_->GetTransform().translation_, positions);
		test_->GetTransform().translation_ = positions[count];
	}
	ImGui::End();

	time += 0.1f;
	if (time > 1.0f && count != positions.size() - 1) {
		time = 0.0f;
		++count;
		test_->GetTransform().translation_ = positions[count];
	}
	test_->Update();*/

	player_->Update();

	enemySpawnerFactory_->Update();

	fieldObjectFactory_->Update();

	gameCamera_->Update();

	skyBox_->Update();

	ParticleManager::GetInstance()->Update();

	sceneFade_->Update();

	// プレイヤーが死んだかクリアをしたらフェードをする
	uint32_t clearKill = static_cast<uint32_t>(player_->GetItem()->GetPlayerData().clearKill);
	if ((!player_->GetIsAlive() || player_->GetShot()->GetNockdownCount() >= clearKill) && !isFade_) {
		isFade_ = true;
		sceneFade_->FadeOut();
	}
	// フェードが終わったらシーン遷移する
	if (isFade_ && !sceneFade_->GetIsFade()) {
		if (!player_->GetIsAlive()) {
			SceneManager::GetInstance()->ChangeScene("Over");
		} else {
			SceneManager::GetInstance()->ChangeScene("Clear");
		}
	}
}

void GamePlayScene::Draw()
{
	player_->EffectDraw();

	enemySpawnerFactory_->Draw();

	player_->Draw();

	sceneFade_->Draw();
}