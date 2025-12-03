#include "LightManager.h"

#ifdef ENABLE_EDITOR
#include "imgui.h"
#endif // ENABLE_EDITOR

#include "DirectXEngine.h"

#include "type/DirectionalLight.h"
#include "type/PointLight.h"
#include "type/SpotLight.h"

std::unique_ptr<LightManager> LightManager::instance_ = nullptr;

LightManager* LightManager::GetInstance()
{
	if (instance_ == nullptr) {
		instance_ = std::make_unique<LightManager>();
	}
	return instance_.get();
}

void LightManager::Initialize(DirectXEngine* dxEngine)
{
	this->dxEngine_ = dxEngine;

	directionalLight_ = std::make_unique<DirectionalLight>();
	directionalLight_->Initialize(dxEngine_);

	pointLight_ = std::make_unique<PointLight>();
	pointLight_->Initialize(dxEngine_);

	spotLight_ = std::make_unique<SpotLight>();
	spotLight_->Initialize(dxEngine_);
}

void LightManager::Update()
{
	directionalLight_->Update();

	pointLight_->Update();

	spotLight_->Update();
}

void LightManager::Debug_ImGui()
{
#ifdef ENABLE_EDITOR
	ImGuiTreeNodeFlags flag = ImGuiTreeNodeFlags_DefaultOpen;
	ImGui::PushItemWidth(150);
	if (ImGui::TreeNodeEx("Light", flag)) {
		if (ImGui::TreeNodeEx("Directional Light", flag)) {
			directionalLight_->Debug_ImGui();
			ImGui::TreePop();
		}
		if (ImGui::TreeNodeEx("Point Light", flag)) {
			pointLight_->Debug_ImGui();
			ImGui::TreePop();
		}
		if (ImGui::TreeNodeEx("Spot Light", flag)) {
			spotLight_->Debug_ImGui();
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
#endif // ENABLE_EDITOR
}

void LightManager::Finalize()
{
	instance_ = nullptr;
}
