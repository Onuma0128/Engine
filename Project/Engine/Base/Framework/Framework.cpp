#include "Framework.h"

#include "SceneManager.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "AudioManager.h"

#include "Input.h"

void Framework::Initialize()
{
    winApp_ = std::make_unique<NumaEngine::WinApp>();
	winApp_->Initialize();

	imGuiManager_ = std::make_unique<NumaEngine::ImGuiManager>();

    directXEngine_ = std::make_unique<NumaEngine::DirectXEngine>();
	directXEngine_->Initialize(winApp_.get(), imGuiManager_.get());

	Input::GetInstance()->Initialize(winApp_.get());

	LoadFiles();
}

void Framework::Finalize()
{
	Input::GetInstance()->Finalize();

	NumaEngine::SceneManager::GetInstance()->Finalize();
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
	TextureManager::GetInstance()->LoadTexture("Resources/Texture", "levelUpLine.png");
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
	TextureManager::GetInstance()->LoadTexture("Resources/Texture", "menuUI.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Texture", "Apple.dds");
	TextureManager::GetInstance()->LoadTexture("Resources/Texture", "noiseTexture.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Texture", "gradationTexture.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Texture", "selectBackGround.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Texture", "muscleTexture.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Texture", "needMoreMacho.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Texture", "nextArrow.png");

	TextureManager::GetInstance()->LoadTexture("Resources/Texture/game", "bossHpBar.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Texture/game", "bossHpFrame.png");

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
	TextureManager::GetInstance()->LoadTexture("Resources/Texture/select", "rematchUI.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Texture/select", "gameClearUI.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Texture/select", "gameOverUI.png");

	// ============================== Model ============================== //

    NumaEngine::ModelManager::GetInstance()->LoadModel("Resources/Model", "plane.obj");
	NumaEngine::ModelManager::GetInstance()->LoadModel("Resources/Model", "Box.obj");
	NumaEngine::ModelManager::GetInstance()->LoadModel("Resources/Model/shadow", "shadow.obj");

	NumaEngine::ModelManager::GetInstance()->LoadModel("Resources/Model/fieldObject", "fence.obj");
	NumaEngine::ModelManager::GetInstance()->LoadModel("Resources/Model/fieldObject", "ground.obj");
	NumaEngine::ModelManager::GetInstance()->LoadModel("Resources/Model/fieldObject", "TowerWindmill.obj");
	NumaEngine::ModelManager::GetInstance()->LoadModel("Resources/Model/fieldObject", "DeadTree_1.gltf");
	NumaEngine::ModelManager::GetInstance()->LoadModel("Resources/Model/fieldObject", "DeadTree_2.gltf");
	NumaEngine::ModelManager::GetInstance()->LoadModel("Resources/Model/fieldObject", "DeadTree_3.gltf");
	NumaEngine::ModelManager::GetInstance()->LoadModel("Resources/Model/fieldObject", "DeadTree_4.gltf");
	NumaEngine::ModelManager::GetInstance()->LoadModel("Resources/Model/fieldObject", "DeadTree_5.gltf");
	NumaEngine::ModelManager::GetInstance()->LoadModel("Resources/Model/fieldObject", "BirchTree_1.obj");
	NumaEngine::ModelManager::GetInstance()->LoadModel("Resources/Model/fieldObject", "Well.obj");
	NumaEngine::ModelManager::GetInstance()->LoadModel("Resources/Model/fieldObject", "OpenBarn.obj");
	NumaEngine::ModelManager::GetInstance()->LoadModel("Resources/Model/fieldObject", "BigBarn.obj");
	NumaEngine::ModelManager::GetInstance()->LoadModel("Resources/Model/fieldObject", "Bush.obj");
	NumaEngine::ModelManager::GetInstance()->LoadModel("Resources/Model/fieldObject", "ShortStoneWall.obj");
	NumaEngine::ModelManager::GetInstance()->LoadModel("Resources/Model/fieldObject", "StoneWall.obj");
	NumaEngine::ModelManager::GetInstance()->LoadModel("Resources/Model/fieldObject", "Rock1.obj");

	NumaEngine::ModelManager::GetInstance()->LoadModel("Resources/Model/player", "Player.gltf");
	NumaEngine::ModelManager::GetInstance()->LoadModel("Resources/Model/muscleCompanion", "muscleCompanion.gltf");
	NumaEngine::ModelManager::GetInstance()->LoadModel("Resources/Model/enemy/shield", "Shield_Heater.obj");
	NumaEngine::ModelManager::GetInstance()->LoadModel("Resources/Model/enemy", "Zombie_Basic.gltf");
	NumaEngine::ModelManager::GetInstance()->LoadModel("Resources/Model/enemy", "Characters_Shaun.gltf");
	NumaEngine::ModelManager::GetInstance()->LoadModel("Resources/Model/Boss", "BossEnemy.gltf");
	NumaEngine::ModelManager::GetInstance()->LoadModel("Resources/Model/Mattyo", "Mattyo.gltf");

	// ============================== Audio ============================== //

    NumaEngine::AudioManager::GetInstance()->LoadAudioFile("Resources/Audio/Scene", "GameSceneBGM.wav");
	NumaEngine::AudioManager::GetInstance()->LoadAudioFile("Resources/Audio/Scene", "GameClearBGM.wav");
	NumaEngine::AudioManager::GetInstance()->LoadAudioFile("Resources/Audio/Scene", "BossBGM.wav");

	NumaEngine::AudioManager::GetInstance()->LoadAudioFile("Resources/Audio/Mattyo", "MattyoDashHit.wav");
	NumaEngine::AudioManager::GetInstance()->LoadAudioFile("Resources/Audio/Mattyo", "MattyoFootsteps.wav");
	NumaEngine::AudioManager::GetInstance()->LoadAudioFile("Resources/Audio/Mattyo", "MattyoGetDamage.wav");
	NumaEngine::AudioManager::GetInstance()->LoadAudioFile("Resources/Audio/Mattyo", "MattyoGiveDamage.wav");
	NumaEngine::AudioManager::GetInstance()->LoadAudioFile("Resources/Audio/Mattyo", "MattyoSet.wav");
	NumaEngine::AudioManager::GetInstance()->LoadAudioFile("Resources/Audio/Mattyo", "MattyoShot.wav");
	NumaEngine::AudioManager::GetInstance()->LoadAudioFile("Resources/Audio/Mattyo", "MattyoClear.wav");
	NumaEngine::AudioManager::GetInstance()->LoadAudioFile("Resources/Audio/Mattyo", "MattyoShield.wav");
	NumaEngine::AudioManager::GetInstance()->LoadAudioFile("Resources/Audio/Mattyo", "MattyoPowerUp.wav");

	NumaEngine::AudioManager::GetInstance()->LoadAudioFile("Resources/Audio/Boss", "BossAttackCaveat.wav");
	NumaEngine::AudioManager::GetInstance()->LoadAudioFile("Resources/Audio/Boss", "BossAttackDash.wav");
	NumaEngine::AudioManager::GetInstance()->LoadAudioFile("Resources/Audio/Boss", "BossDown.wav");
	NumaEngine::AudioManager::GetInstance()->LoadAudioFile("Resources/Audio/Boss", "BossEnemySpawn.wav");
	NumaEngine::AudioManager::GetInstance()->LoadAudioFile("Resources/Audio/Boss", "BossLanding.wav");
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
