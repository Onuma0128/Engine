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

	test = std::make_unique<ParticleEmitter>("particleName");
	particleManager->CreateParticleGroup(test);
	test->SetIsCreate(false);

	skyBox_ = std::make_unique<PrimitiveDrawr>();
	skyBox_->TypeInit(PrimitiveType::Skybox);
	skyBox_->GetTransform().scale = { 1024.0f,1024.0f ,1024.0f };
	skyBox_->SetSceneRenderer();

	for (uint32_t i = 0; i < 5; ++i) {
		for (uint32_t j = 0; j < 5; ++j) {
			std::unique_ptr<Animation> animation = std::make_unique<Animation>();
			animation->Initialize("BrainStem.gltf");
			animation->SetSceneRenderer();
			float posX = (static_cast<float>(i) - 4.0f);
			float posZ = (static_cast<float>(j) - 4.0f);
			EulerTransform transform = { {1.0f,1.0f,1.0f},{0.0f,180.0f,0.0f},{posX,1.0f,posZ} };
			animation->GetTransform().rotation_ = Vector3::FromEuler(transform.rotation);
			animation->GetTransform().translation_ = transform.translation;
			float time = static_cast<float>(i) / 8.0f + static_cast<float>(j) / 8.0f;
			animation->SetAnimationTime(time);
			animation->GetMaterial().environmentCoefficient = time;
			testAnimas_.push_back(std::move(animation));
		}
	}

	box_ = std::make_unique<Object3d>();
	box_->Initialize("Box.obj");
	box_->SetSceneRenderer();
	box_->GetTransform().translation_ = { 2.5f,2.0f,-2.0f };
	box_->GetMaterial().environmentCoefficient = 0.5f;
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

	skyBox_->Update();

	t += 0.005f;
	box_->GetTransform().rotation_ = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::ExprUnitY, t);
	box_->Update();

	for (auto& animation : testAnimas_) {
		animation->Update();
	}

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
	sceneFade_->Draw();
}
