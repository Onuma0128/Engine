#include "Framework.h"

#include "SceneManager.h"
#include "ModelManager.h"
#include "TextureManager.h"

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
	TextureManager::GetInstance()->LoadTexture("resources/texture", "smoke.png");
	TextureManager::GetInstance()->LoadTexture("resources/texture", "circle.png");
	TextureManager::GetInstance()->LoadTexture("resources/texture", "woodChip.png");
	TextureManager::GetInstance()->LoadTexture("resources", "gradationLine.png");
	TextureManager::GetInstance()->LoadTexture("resources", "white1x1.png");
	TextureManager::GetInstance()->LoadTexture("resources", "uvChecker.png");
	TextureManager::GetInstance()->LoadTexture("resources", "output.dds");
	TextureManager::GetInstance()->LoadTexture("resources/player/texture", "2dReticle.png");
	TextureManager::GetInstance()->LoadTexture("resources/player/ui", "bulletUI.png");
	//TextureManager::GetInstance()->LoadTexture("resources/fieldObject", "ground.png");
	TextureManager::GetInstance()->LoadTexture("resources/texture", "blood.png");

	ModelManager::GetInstance()->LoadModel("resources", "terrain.obj");
	ModelManager::GetInstance()->LoadModel("resources", "sphere.obj");
	ModelManager::GetInstance()->LoadModel("resources", "plane.gltf");
	ModelManager::GetInstance()->LoadModel("resources", "plane.obj");
	ModelManager::GetInstance()->LoadModel("resources", "suzanne.obj");
	ModelManager::GetInstance()->LoadModel("resources", "Box.obj");
	ModelManager::GetInstance()->LoadModel("resources/player/revolver", "Revolver.obj");

	ModelManager::GetInstance()->LoadModel("resources/fieldObject", "ground.obj");
	ModelManager::GetInstance()->LoadModel("resources/fieldObject", "DeadTree_1.gltf");
	ModelManager::GetInstance()->LoadModel("resources/fieldObject", "DeadTree_2.gltf");
	ModelManager::GetInstance()->LoadModel("resources/fieldObject", "DeadTree_3.gltf");
	ModelManager::GetInstance()->LoadModel("resources/fieldObject", "DeadTree_4.gltf");
	ModelManager::GetInstance()->LoadModel("resources/fieldObject", "DeadTree_5.gltf");
	ModelManager::GetInstance()->LoadModel("resources/fieldObject", "BirchTree_1.obj");

	ModelManager::GetInstance()->LoadModel("resources/enemy", "Zombie_Basic.gltf");
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
