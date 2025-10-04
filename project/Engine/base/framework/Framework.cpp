#include "Framework.h"

#include "SceneManager.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "AudioManager.h"

#include "Input.h"

void Framework::Initialize()
{
	winApp_ = std::make_unique<WinApp>();
	winApp_->Initialize();

	imGuiManager_ = std::make_unique<ImGuiManager>();

	directXEngine_ = std::make_unique<DirectXEngine>();
	directXEngine_->Initialize(winApp_.get(), imGuiManager_.get());

	Input::GetInstance()->Initialize(winApp_.get());

	LoadFiles();
}

void Framework::Finalize()
{
	Input::GetInstance()->Finalize();

	SceneManager::GetInstance()->Finalize();
}

void Framework::Update()
{
	//Windowにメッセージが来てたら最優先で処理させる
	if (winApp_->ProcessMessage()) {
		endRequst_ = true;
	}
	else {
		// 入力の更新
		Input::GetInstance()->Update();

	}
}

void Framework::LoadFiles()
{
	// ============================== Texture ============================== //

	TextureManager::GetInstance()->LoadTexture("resources/texture", "smoke.png");
	TextureManager::GetInstance()->LoadTexture("resources/texture", "circle.png");
	TextureManager::GetInstance()->LoadTexture("resources/texture", "woodChip.png");
	TextureManager::GetInstance()->LoadTexture("resources/texture", "gradationLine.png");
	TextureManager::GetInstance()->LoadTexture("resources/texture", "white1x1.png");
	TextureManager::GetInstance()->LoadTexture("resources/texture", "uvChecker.png");
	TextureManager::GetInstance()->LoadTexture("resources/texture", "output.dds");
	TextureManager::GetInstance()->LoadTexture("resources/texture", "blood.png");
	TextureManager::GetInstance()->LoadTexture("resources/texture", "numbers.png");
	TextureManager::GetInstance()->LoadTexture("resources/texture", "titleUI.png");
	TextureManager::GetInstance()->LoadTexture("resources/texture", "gameClearUI.png");
	TextureManager::GetInstance()->LoadTexture("resources/texture", "gameOverUI.png");
	TextureManager::GetInstance()->LoadTexture("resources/texture", "menuUI.png");

	TextureManager::GetInstance()->LoadTexture("resources/texture/player", "2dReticle.png");
	TextureManager::GetInstance()->LoadTexture("resources/texture/player", "bulletUI.png");
	TextureManager::GetInstance()->LoadTexture("resources/texture/player", "playerControlUI.png");

	// ============================== Model ============================== //

	ModelManager::GetInstance()->LoadModel("resources/model", "plane.obj");
	ModelManager::GetInstance()->LoadModel("resources/model", "Box.obj");
	ModelManager::GetInstance()->LoadModel("resources/model/shadow", "shadow.obj");

	ModelManager::GetInstance()->LoadModel("resources/model/fieldObject", "fence.obj");
	ModelManager::GetInstance()->LoadModel("resources/model/fieldObject", "ground.obj");
	ModelManager::GetInstance()->LoadModel("resources/model/fieldObject", "TowerWindmill.obj");
	ModelManager::GetInstance()->LoadModel("resources/model/fieldObject", "DeadTree_1.gltf");
	ModelManager::GetInstance()->LoadModel("resources/model/fieldObject", "DeadTree_2.gltf");
	ModelManager::GetInstance()->LoadModel("resources/model/fieldObject", "DeadTree_3.gltf");
	ModelManager::GetInstance()->LoadModel("resources/model/fieldObject", "DeadTree_4.gltf");
	ModelManager::GetInstance()->LoadModel("resources/model/fieldObject", "DeadTree_5.gltf");
	ModelManager::GetInstance()->LoadModel("resources/model/fieldObject", "BirchTree_1.obj");
	ModelManager::GetInstance()->LoadModel("resources/model/fieldObject", "Well.obj");
	ModelManager::GetInstance()->LoadModel("resources/model/fieldObject", "OpenBarn.obj");
	ModelManager::GetInstance()->LoadModel("resources/model/fieldObject", "BigBarn.obj");
	ModelManager::GetInstance()->LoadModel("resources/model/fieldObject", "Bush.obj");
	ModelManager::GetInstance()->LoadModel("resources/model/fieldObject", "ShortStoneWall.obj");
	ModelManager::GetInstance()->LoadModel("resources/model/fieldObject", "StoneWall.obj");

	ModelManager::GetInstance()->LoadModel("resources/model/player", "Player.gltf");
	ModelManager::GetInstance()->LoadModel("resources/model/enemy/shield", "Shield_Heater.obj");
	ModelManager::GetInstance()->LoadModel("resources/model/enemy", "Zombie_Basic.gltf");
	ModelManager::GetInstance()->LoadModel("resources/model/enemy", "Characters_Shaun.gltf");

	// ============================== Audio ============================== //

	AudioManager::GetInstance()->LoadAudioFile("resources/audio", "mokugyo.wav");
}

void Framework::Run()
{
	Initialize();

	//ウィンドウの×ボタンが押されるまでループ
	while (true) {
		// 更新
		Update();
		// 呼び出されたらループ終了
		if (IsEndRequst()) {
			break;
		}
		// 描画
		Draw();
	}
	// 終了
	Finalize();
}
