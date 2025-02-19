#include "GamePlayScene.h"

#include "ParticleManager.h"
#include "SceneManager.h"
#include "ModelManager.h"
#include "Input.h"

#include "imgui.h"
#include "titleScene/TitleScene.h"

void GamePlayScene::Initialize()
{
	testObj_ = std::make_unique<TestObject>();
	testObj_->Init();

	terrain_ = std::make_unique<Object3d>();
	terrain_->Initialize("terrain.obj");

	sphere_ = std::make_unique<Object3d>();
	sphere_->Initialize("sphere.obj");
	sphere_->GetTransform().translation_ = {1.5f,1.0f,0.0f};

	plane_ = std::make_unique<Object3d>();
	plane_->Initialize("plane.gltf");
	plane_->GetTransform().translation_ = {-1.5f,1.0f,0.0f};
	planeAngle_.y = 3.14f;

	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize("uvChecker.png");
	sprite_->SetSize({ 200.0f,200.0f });

	emitter_ = std::make_unique<ParticleEmitter>("test");
	ParticleManager::GetInstance()->CreateParticleGroup("test", "uvChecker.png", emitter_.get());

	sphereEffect_ = std::make_unique<TrailEffect>();
	sphereEffect_->InitSphere(5);
	sphereEffect_->SetTexcoordX_Alpha(false);
	sphereEffect_->SetTexcoordY_Alpha(false);
	sphereEffect_->GetTransform().scale = Vector3{ 1.0f,0.2f,1.0f };
}

void GamePlayScene::Finalize()
{
}

void GamePlayScene::Update()
{
	Transform transform = sphereEffect_->GetTransform();
	Vector4 color = sphereEffect_->GetColor();
	ImGui::Begin("models");
	ImGuiTreeNodeFlags flag = ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::TreeNodeEx("objectTransform", flag)) {
		if (ImGui::TreeNodeEx("terrain", flag)) {
			ImGui::DragFloat3("scale", &terrain_->GetTransform().scale_.x, 0.01f);
			ImGui::DragFloat3("rotation", &terrainAngle_.x, 0.01f);
			ImGui::DragFloat3("translation", &terrain_->GetTransform().translation_.x, 0.01f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNodeEx("sphere", flag)) {
			ImGui::DragFloat3("scale", &sphere_->GetTransform().scale_.x, 0.01f);
			ImGui::DragFloat3("rotation", &sphereAngle_.x, 0.01f);
			ImGui::DragFloat3("translation", &sphere_->GetTransform().translation_.x, 0.01f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNodeEx("plane", flag)) {
			ImGui::DragFloat3("scale", &plane_->GetTransform().scale_.x, 0.01f);
			ImGui::DragFloat3("rotation", &planeAngle_.x, 0.01f);
			ImGui::DragFloat3("translation", &plane_->GetTransform().translation_.x, 0.01f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNodeEx("sphereEffect", flag)) {
			ImGui::DragFloat3("scale", &transform.scale.x, 0.01f);
			ImGui::DragFloat3("rotation", &transform.rotation.x, 0.01f);
			ImGui::DragFloat3("translation", &transform.translation.x, 0.01f);
			ImGui::ColorEdit4("color", &color.x);
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
	ImGui::End();

	// 回転を計算
	AddQuaternion(terrain_->GetTransform().rotation_, terrainAngle_);
	AddQuaternion(sphere_->GetTransform().rotation_, sphereAngle_);
	AddQuaternion(plane_->GetTransform().rotation_, planeAngle_);
	sphereEffect_->SetTransform(transform);
	sphereEffect_->SetColor({ color.x,color.y,color.z });
	sphereEffect_->SetAlpha(color.w);

	testObj_->Update();
	terrain_->Update();
	sphere_->Update();
	plane_->Update();
	sprite_->Update();

	sphereEffect_->Update();

	ParticleManager::GetInstance()->Update();
}

void GamePlayScene::Draw()
{	
	testObj_->Draw();
	terrain_->Draw();
	sphere_->Draw();
	plane_->Draw();
	
	sprite_->Draw();

	sphereEffect_->DrawSphere();

	ParticleManager::GetInstance()->Draw();
}

void GamePlayScene::AddQuaternion(Quaternion& quaternion, const Vector3& angle)
{
	Quaternion addQuaternionX = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::ExprUnitX, angle.x);
	Quaternion addQuaternionY = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::ExprUnitY, angle.y);
	Quaternion addQuaternionZ = Quaternion::MakeRotateAxisAngleQuaternion(Vector3::ExprUnitZ, angle.z);
	quaternion = (addQuaternionX * addQuaternionY * addQuaternionZ);
}
