#include "TitleScene.h"

#include "DirectXEngine.h"
#include "SceneManager.h"
#include "CameraManager.h"
#include "Input.h"
#include "SceneJsonLoader.h"
#include "PostEffectManager.h"

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

	// シーンのロード
	SceneJsonLoader loader;
	loader.Load("sceneObject");

	// フィールド上のオブジェクトの初期化と生成
	fieldObjectFactory_ = std::make_unique<FieldObjectFactory>();
	fieldObjectFactory_->Init(loader);
	DirectXEngine::GetPostEffectMgr()->CreatePostEffect(PostEffectType::kOutLine);

	test = std::make_unique<ParticleEmitter>("playerHit");
	particleManager->CreateParticleGroup(test);
	test->SetIsCreate(false);
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

	fieldObjectFactory_->Update();

	particleManager->Update();

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
	titleUI_->Draw();

	sceneFade_->Draw();
}
