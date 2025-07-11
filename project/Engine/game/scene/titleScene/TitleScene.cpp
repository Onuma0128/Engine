#include "TitleScene.h"

#include "SceneManager.h"
#include "CameraManager.h"
#include "Input.h"

void TitleScene::Initialize()
{
	// カメラの初期化
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
	camera_->SetRotation(Vector3{ 0.55f,0.0f,0.0f });
	camera_->SetTranslation(Vector3{ -0.4f,10.0f,-15.0f });
	CameraManager::GetInstance()->Clear();
	CameraManager::GetInstance()->SetCamera(camera_.get());
	CameraManager::GetInstance()->SetActiveCamera(0);
	camera_->Update();

	sceneFade_ = std::make_unique<SceneFade>();
	sceneFade_->Init();
	sceneFade_->FadeIn(2.0f);

	titleUI_ = std::make_unique<TitleUI>();
	titleUI_->Init();

	for (uint32_t i = 0; i < 8; ++i) {
		for (uint32_t j = 0; j < 8; ++j) {
			std::unique_ptr<Animation> animation = std::make_unique<Animation>();
			animation->Initialize("BrainStem.gltf");
			animation->SetSceneRenderer();
			float posX = (static_cast<float>(i) - 4.0f);
			float posZ = (static_cast<float>(j) - 4.0f);
			EulerTransform transform = { {1.0f,1.0f,1.0f},{0.0f,180.0f,0.0f},{posX,1.0f,posZ} };
			animation->GetTransform().rotation_ = Vector3::FromEuler(transform.rotation);
			animation->GetTransform().translation_ = transform.translation;
			float time = static_cast<float>(i) / 14.0f + static_cast<float>(j) / 14.0f;
			animation->SetAnimationTime(time);
			animation->SetColor(Vector4{ 1.0f ,time ,time ,1.0f });
			testAnimas_.push_back(std::move(animation));
		}
	}
}

void TitleScene::Finalize()
{
}

void TitleScene::Update()
{
	Input* input = Input::GetInstance();

	camera_->Update();

	titleUI_->Update();

	sceneFade_->Update();

	for (auto& animation : testAnimas_) {
		animation->Update();
	}

    if ((input->TriggerGamepadButton(XINPUT_GAMEPAD_A) || input->TriggerKey(DIK_SPACE)) && !isFade_) {
		isFade_ = true;
		sceneFade_->FadeOut();
	}
	// フェードが終わったらシーン遷移する
	if (isFade_ && !sceneFade_->GetIsFade()) {
		SceneManager::GetInstance()->ChangeScene("Game");
	}
}

void TitleScene::Draw()
{
	sceneFade_->Draw();
}
