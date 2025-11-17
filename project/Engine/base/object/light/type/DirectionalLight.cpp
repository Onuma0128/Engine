#include "DirectionalLight.h"

#include <DirectXMath.h>

#include "DirectXEngine.h"
#include "imgui.h"

#include "CreateBufferResource.h"

void DirectionalLight::Initialize(DirectXEngine* dxEngine)
{
	dxEngine_ = dxEngine;
	MakeLightData();

	center_ = { -10.0f,35.0f,15.0f };
}

void DirectionalLight::Update()
{
	lightData_->direction = lightData_->direction.Normalize();

    const Vector3 sceneMin = { -30.0f, -5.0f, -30.0f };
    const Vector3 sceneMax = { +30.0f, +15.0f, +30.0f };
    const uint32_t shadowW = 2048;
    const uint32_t shadowH = 2048;
    BuildMatricesCoverAll(sceneMin, sceneMax, shadowW, shadowH);
}

void DirectionalLight::Debug_ImGui()
{
	ImGui::ColorEdit4("LightColor", (float*)&lightData_->color.x);
	ImGui::DragFloat3("DirectionalLightData.Direction", &lightData_->direction.x, 0.01f);
	ImGui::DragFloat("DirectionalLightData.intensity", &lightData_->intensity, 0.01f);

	ImGui::DragFloat("Radius", &radius_, 0.01f, 0.1f, 1000000.0f);
	ImGui::DragFloat("RadiusScale", &radiusScale_, 0.01f, 1.0f, 1000000.0f);
	ImGui::DragFloat3("Center", &center_.x, 0.01f);

	auto& M = lightMatrixs_.lightVP;
    ImGui::Text("Row2: %f %f %f %f", M.m[2][0], M.m[2][1], M.m[2][2], M.m[2][3]);
}

void DirectionalLight::MakeLightData()
{
	resource_ = CreateBufferResource(dxEngine_->GetDevice(), sizeof(DirectionalLightData));
	bufferView_.BufferLocation = resource_->GetGPUVirtualAddress();
	bufferView_.SizeInBytes = sizeof(DirectionalLightData);
	bufferView_.StrideInBytes = sizeof(DirectionalLightData);
	resource_->Map(0, nullptr, reinterpret_cast<void**>(&lightData_));
	// デフォルト値
	lightData_->color = { 1.0f,0.84f,0.67f,1.0f };
	lightData_->direction = { 0.0f,1.0f,0.0f };
	lightData_->intensity = 2.0f;
}

// マップ全体を覆う AABB を与える前提（sceneMin/sceneMax）
void DirectionalLight::BuildMatricesCoverAll(
    const Vector3& sceneMin,
    const Vector3& sceneMax,
    uint32_t shadowW, uint32_t shadowH)
{
	DirectX::XMMATRIX dxProjectionMatrix = DirectX::XMMatrixOrthographicLH(
		80.0f,
		80.0f,
		1.0f,
		640.0f
	);

	DirectX::XMFLOAT3 eye(center_.x, center_.y, center_.z);
	DirectX::XMFLOAT3 target(-lightData_->direction.x, -lightData_->direction.y, -lightData_->direction.z);
	DirectX::XMFLOAT3 up(0.0f, 0.0f, -1.0f);

	DirectX::XMMATRIX dxViewMatrix = DirectX::XMMatrixLookAtLH(
		DirectX::XMLoadFloat3(&eye),
		DirectX::XMLoadFloat3(&target),
		DirectX::XMLoadFloat3(&up));

	// 元のMatrix4x4に戻す
	DirectX::XMStoreFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4*>(&lightMatrixs_.lightProj), dxProjectionMatrix);
	DirectX::XMStoreFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4*>(&lightMatrixs_.lightView), dxViewMatrix);

	lightMatrixs_.lightVP = lightMatrixs_.lightView * lightMatrixs_.lightProj;
}
