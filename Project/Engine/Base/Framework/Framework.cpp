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

	TextureManager::GetInstance()->LoadTexture("Resources/Texture", "star.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Texture", "gradationWhite.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Texture", "downLine.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Texture", "smoke.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Texture", "circle.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Texture", "circleWhite.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Texture", "woodChip.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Texture", "gradationLine.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Texture", "white1x1.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Texture", "uvChecker.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Texture", "output.dds");
	TextureManager::GetInstance()->LoadTexture("Resources/Texture", "blood.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Texture", "numbers.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Texture", "titleUI.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Texture", "gameClearUI.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Texture", "gameOverUI.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Texture", "menuUI.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Texture", "Apple.dds");
	TextureManager::GetInstance()->LoadTexture("Resources/Texture", "noiseTexture.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Texture", "gradationTexture.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Texture", "selectBackGround.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Texture", "muscleTexture.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Texture", "needMoreMacho.png");

	TextureManager::GetInstance()->LoadTexture("Resources/Texture/player", "2dReticle.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Texture/player", "bulletUI.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Texture/player", "playerControlUI.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Texture/player", "reloadUI.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Texture/player", "A_button.png");

	TextureManager::GetInstance()->LoadTexture("Resources/Texture/select", "killCountUI.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Texture/select", "hitRateUI.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Texture/select", "selectTitleUI.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Texture/select", "selectOnceAgainUI.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Texture/select", "countUI.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Texture/select", "ratioUI.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Texture/select", "catUI.png");

	// ============================== Model ============================== //

	ModelManager::GetInstance()->LoadModel("Resources/Model", "plane.obj");
	ModelManager::GetInstance()->LoadModel("Resources/Model", "Box.obj");
	ModelManager::GetInstance()->LoadModel("Resources/Model/shadow", "shadow.obj");

	ModelManager::GetInstance()->LoadModel("Resources/Model/fieldObject", "fence.obj");
	ModelManager::GetInstance()->LoadModel("Resources/Model/fieldObject", "ground.obj");
	ModelManager::GetInstance()->LoadModel("Resources/Model/fieldObject", "TowerWindmill.obj");
	ModelManager::GetInstance()->LoadModel("Resources/Model/fieldObject", "DeadTree_1.gltf");
	ModelManager::GetInstance()->LoadModel("Resources/Model/fieldObject", "DeadTree_2.gltf");
	ModelManager::GetInstance()->LoadModel("Resources/Model/fieldObject", "DeadTree_3.gltf");
	ModelManager::GetInstance()->LoadModel("Resources/Model/fieldObject", "DeadTree_4.gltf");
	ModelManager::GetInstance()->LoadModel("Resources/Model/fieldObject", "DeadTree_5.gltf");
	ModelManager::GetInstance()->LoadModel("Resources/Model/fieldObject", "BirchTree_1.obj");
	ModelManager::GetInstance()->LoadModel("Resources/Model/fieldObject", "Well.obj");
	ModelManager::GetInstance()->LoadModel("Resources/Model/fieldObject", "OpenBarn.obj");
	ModelManager::GetInstance()->LoadModel("Resources/Model/fieldObject", "BigBarn.obj");
	ModelManager::GetInstance()->LoadModel("Resources/Model/fieldObject", "Bush.obj");
	ModelManager::GetInstance()->LoadModel("Resources/Model/fieldObject", "ShortStoneWall.obj");
	ModelManager::GetInstance()->LoadModel("Resources/Model/fieldObject", "StoneWall.obj");
	ModelManager::GetInstance()->LoadModel("Resources/Model/fieldObject", "Rock1.obj");

	ModelManager::GetInstance()->LoadModel("Resources/Model/player", "Player.gltf");
	ModelManager::GetInstance()->LoadModel("Resources/Model/muscleCompanion", "muscleCompanion.gltf");
	ModelManager::GetInstance()->LoadModel("Resources/Model/enemy/shield", "Shield_Heater.obj");
	ModelManager::GetInstance()->LoadModel("Resources/Model/enemy", "Zombie_Basic.gltf");
	ModelManager::GetInstance()->LoadModel("Resources/Model/enemy", "Characters_Shaun.gltf");
	ModelManager::GetInstance()->LoadModel("Resources/Model/Boss", "BossEnemy.gltf");
	ModelManager::GetInstance()->LoadModel("Resources/Model/Mattyo", "Mattyo.gltf");

	// ============================== Audio ============================== //

	AudioManager::GetInstance()->LoadAudioFile("Resources/Audio/Scene", "GameSceneBGM.wav");
	AudioManager::GetInstance()->LoadAudioFile("Resources/Audio", "mokugyo.wav");

	AudioManager::GetInstance()->LoadAudioFile("Resources/Audio/Mattyo", "MattyoDashHit.wav");
	AudioManager::GetInstance()->LoadAudioFile("Resources/Audio/Mattyo", "MattyoFootsteps.wav");
	AudioManager::GetInstance()->LoadAudioFile("Resources/Audio/Mattyo", "MattyoGetDamage.wav");
	AudioManager::GetInstance()->LoadAudioFile("Resources/Audio/Mattyo", "MattyoGiveDamage.wav");
	AudioManager::GetInstance()->LoadAudioFile("Resources/Audio/Mattyo", "MattyoSet.wav");
	AudioManager::GetInstance()->LoadAudioFile("Resources/Audio/Mattyo", "MattyoShot.wav");

	AudioManager::GetInstance()->LoadAudioFile("Resources/Audio/Boss", "BossAttackCaveat.wav");
	AudioManager::GetInstance()->LoadAudioFile("Resources/Audio/Boss", "BossAttackDash.wav");
	AudioManager::GetInstance()->LoadAudioFile("Resources/Audio/Boss", "BossDown.wav");
	AudioManager::GetInstance()->LoadAudioFile("Resources/Audio/Boss", "BossEnemySpawn.wav");
	AudioManager::GetInstance()->LoadAudioFile("Resources/Audio/Boss", "BossLanding.wav");
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
