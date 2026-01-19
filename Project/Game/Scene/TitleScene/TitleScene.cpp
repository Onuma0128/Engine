#include "TitleScene.h"

#include "DirectXEngine.h"
#include "SceneManager.h"
#include "CameraManager.h"
#include "Input.h"
#include "SceneJsonLoader.h"
#include "PostEffectManager.h"

bool TitleScene::isBossStart = false;

void TitleScene::Initialize()
{
	// カメラの初期化
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
	camera_->SetRotation(Vector3{ 0.55f,0.0f,0.0f });
	camera_->SetTranslation(Vector3{ -0.4f,10.0f,-15.0f });
	CameraManager::GetInstance()->Clear();
	CameraManager::GetInstance()->SetCamera(camera_);
	CameraManager::GetInstance()->SetActiveCamera(0);
	camera_->Update();

	sceneFade_ = std::make_unique<BaseUI>();
	sceneFade_->Init("TitleFade", "GameData", true);
	sceneFade_->GetSprite()->SetColor(Vector4{ 0.0f,0.0f,0.0f,1.0f });
	sceneFade_->FadeOut();

	titleUI_ = std::make_unique<TitleUI>();
	titleUI_->Init();
	titleUI_->FadeIn();

	// シーンのロード
	SceneJsonLoader loader;
	loader.Load("sceneObject");

	// フィールド上のオブジェクトの初期化と生成
	fieldObjectFactory_ = std::make_unique<FieldObjectFactory>();
	fieldObjectFactory_->Init(loader);
	PostEffectManager::GetInstance()->CreatePostEffect(PostEffectType::kOutLine);

	test = std::make_unique<ParticleEmitter>("companionDamageUp");
	particleManager->CreateParticleGroup(test);
	test->SetIsCreate(false);

	// BGMを流す
	const float kBGMVolume = 0.04f;
	bgm_ = std::make_unique<Audio>();
	bgm_->SoundPlayWave("GameSceneBGM.wav", kBGMVolume, true);
}

void TitleScene::Finalize()
{
}

void TitleScene::Update()
{
	Input* input = Input::GetInstance();

	camera_->Update();

	titleUI_->Update();

	sceneFade_->DrawImGui();
	sceneFade_->Update();

	fieldObjectFactory_->Update();

	particleManager->Update();

    if ((input->TriggerGamepadButton(XINPUT_GAMEPAD_A) || input->TriggerKey(DIK_SPACE)) && !isFade_) {
		isFade_ = true;
		sceneFade_->FadeIn();
		titleUI_->FadeOut();
	}
	// フェードが終わったらシーン遷移する
	if (isFade_ && !sceneFade_->IsPlayAnimation()) {
		if (input->GetGamepadLeftTrigger() > 0.0f && input->GetGamepadRightTrigger() > 0.0f) {
			TitleScene::isBossStart = true;
		}
		SceneManager::GetInstance()->ChangeScene("Game");
	}
}

void TitleScene::Draw()
{
	titleUI_->Draw();

	sceneFade_->Draw();
}
