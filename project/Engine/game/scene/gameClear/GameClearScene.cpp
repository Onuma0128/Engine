#include "GameClearScene.h"

#include "SceneManager.h"
#include "CameraManager.h"
#include "Input.h"

void GameClearScene::Initialize()
{
	// カメラの初期化
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
	CameraManager::GetInstance()->SetCamera(camera_.get());
	CameraManager::GetInstance()->SetActiveCamera(0);
	camera_->SetTranslation(Vector3{ 0.0f,1.75f,-4.0f });
	camera_->Update();

	sceneFade_ = std::make_unique<SceneFade>();
	sceneFade_->Init();
	sceneFade_->FadeIn(2.0f);

	enemy_ = std::make_unique<Animation>();
	enemy_->Initialize("Zombie_Basic.gltf");
	enemy_->PlayByName("Yes");
	enemy_->SetSceneRenderer();
	enemy_->GetTransform().rotation_ = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::ExprUnitY, 3.14f);

	gameClearUI_ = std::make_unique<GameClearUI>();
	gameClearUI_->Init();
}

void GameClearScene::Finalize()
{
}

void GameClearScene::Update()
{
	Input* input = Input::GetInstance();

	camera_->Update();

	enemy_->Update();

	gameClearUI_->Update();

	sceneFade_->Update();

	if ((input->TriggerKey(DIK_SPACE) || input->TriggerGamepadButton(XINPUT_GAMEPAD_A)) && !isFade_) {
		isFade_ = true;
		sceneFade_->FadeOut();
	}
	// フェードが終わったらシーン遷移する
	if (isFade_ && !sceneFade_->GetIsFade()) {
		SceneManager::GetInstance()->ChangeScene("Title");
	}
}

void GameClearScene::Draw()
{
	sceneFade_->Draw();
}

