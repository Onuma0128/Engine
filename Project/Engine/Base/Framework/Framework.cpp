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

	TextureManager::GetInstance()->LoadTexture("Resources/Texture", "white1x1.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Texture", "uvChecker.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Texture", "numbers.png");

	TextureManager::GetInstance()->LoadTexture("Resources/Texture", "foreArm.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Texture", "upperArm.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Texture", "muscleTexture.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Texture", "cheerForUI.png");
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
