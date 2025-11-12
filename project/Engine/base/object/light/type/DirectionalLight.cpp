#include "DirectionalLight.h"

#include "DirectXEngine.h"
#include "imgui.h"

#include "CreateBufferResource.h"

void DirectionalLight::Initialize(DirectXEngine* dxEngine)
{
	dxEngine_ = dxEngine;
	MakeLightData();
}

void DirectionalLight::Update()
{
	lightData_->direction = lightData_->direction.Normalize();

    const Vector3 sceneMin = { -30.0f, -5.0f, -30.0f };
    const Vector3 sceneMax = { +30.0f, +15.0f, +30.0f };
    const uint32_t shadowW = 2048;
    const uint32_t shadowH = 2048;
    BuildMatricesCoverAll(sceneMin, sceneMax, shadowW, shadowH);

	lightData_->lightVP = lightMatrixs_.lightVP;
}

void DirectionalLight::Debug_ImGui()
{
	ImGui::ColorEdit4("LightColor", (float*)&lightData_->color.x);
	ImGui::DragFloat3("DirectionalLightData.Direction", &lightData_->direction.x, 0.01f);
	ImGui::DragFloat("DirectionalLightData.intensity", &lightData_->intensity, 0.01f);

	ImGui::DragFloat("Radius", &radius_, 0.01f, 0.1f, 1000000.0f);
	ImGui::DragFloat("RadiusScale", &radiusScale_, 0.01f, 1.0f, 1000000.0f);
	ImGui::DragFloat3("Center", &center_.x, 0.01f);
}

void DirectionalLight::MakeLightData()
{
	resource_ = CreateBufferResource(dxEngine_->GetDevice(), sizeof(DirectionalLightData));
	bufferView_.BufferLocation = resource_->GetGPUVirtualAddress();
	bufferView_.SizeInBytes = sizeof(DirectionalLightData);
	bufferView_.StrideInBytes = sizeof(DirectionalLightData);
	resource_->Map(0, nullptr, reinterpret_cast<void**>(&lightData_));
	// デフォルト値
	lightData_->color = { 1.0f,1.0f,1.0f,1.0f };
	lightData_->direction = { 0.0f,0.85f,-0.5f };
	lightData_->intensity = 1.0f;
	lightData_->lightVP = Matrix4x4::Identity();
}

// マップ全体を覆う AABB を与える前提（sceneMin/sceneMax）
void DirectionalLight::BuildMatricesCoverAll(
    const Vector3& sceneMin,
    const Vector3& sceneMax,
    uint32_t shadowW, uint32_t shadowH)
{
    const Vector3 mapCenter = (sceneMin + sceneMax) * 0.5f; // XZ の中心
    const float   mapHalfX = 0.5f * (sceneMax.x - sceneMin.x);
    const float   mapHalfZ = 0.5f * (sceneMax.z - sceneMin.z);

    const float   maxHeight = 30.0f;   // 余裕を持って（10～20H と聞いているので 30）
    const float   eyeHeight = 200.0f;  // とりあえず十分高く
    const Vector3 eye = mapCenter + Vector3{ 0, eyeHeight, 0 };
    const Vector3 target = mapCenter + Vector3{ 0, 0, 0 };
    const Vector3 up = Vector3{ 0, 0, +1 }; // 真上からなので up は +Z

    lightMatrixs_.lightView = Matrix4x4::LookAt(eye, target, up);
    Matrix4x4 flipZ = Matrix4x4::Identity();
    flipZ.m[2][2] = -1.0f;               // Z を反転
    lightMatrixs_.lightView = lightMatrixs_.lightView * flipZ;

    // 2) 正射影範囲… XZ はマップ全体 + パディング、Y(高さ) を Zレンジに入れる
    const float padXY = 20.0f;
    const float halfW = mapHalfX + padXY;
    const float halfH = mapHalfZ + padXY;

    const float nearZ = 0.1f;                         // LH で z∈[0,1]、near は正
    const float farZ = eyeHeight + maxHeight + 200;  // 目から地面+高さ+余裕 くらい

    lightMatrixs_.lightProj = Matrix4x4::Orthographic(-halfW, +halfW, -halfH, +halfH, nearZ, farZ);

    // 3) VP（HLSL が列ベース mul(pos, M) 想定なら Transpose）
    lightMatrixs_.lightVP = (lightMatrixs_.lightView * lightMatrixs_.lightProj);
    if (lightData_) lightData_->lightVP = lightMatrixs_.lightVP;
}

void DirectionalLight::BuildViewByTargetEye(const Vector3& Ptarget, const Vector3& Peye, float lengthScale)
{
    // lightDir は「ライト→シーン」の向きで正規化して使う
    Vector3 dir = lightData_->direction.Normalize();

    // 距離 L をカメラ距離から決める（スケール可）
    float L = (Ptarget - Peye).Length() * std::max(0.0f, lengthScale);
    if (L < 1e-3f) L = 1.0f; // ゼロ除けの保険

    // up はライト方向と近い軸を避けて選ぶ
    Vector3 up = (std::fabs(dir.y) > 0.99f) ? Vector3::ExprUnitZ : Vector3::ExprUnitY;

    // 「Plight = Ptarget - dir * L」 で便宜的なライト位置
    Vector3 eye = Ptarget - dir * L;

    // View を作成（あなたの行列規約に合わせた LookAt）
    lightMatrixs_.lightView = Matrix4x4::LookAt(eye, Ptarget, up);
}

void DirectionalLight::BuildViewByTargetLength(const Vector3& Ptarget, float L)
{
    Vector3 dir = lightData_->direction.Normalize();
    if (L < 1e-3f) L = 1.0f;

    Vector3 up = (std::fabs(dir.y) > 0.99f) ? Vector3::ExprUnitZ : Vector3::ExprUnitY;
    Vector3 eye = Ptarget - dir * L;

    lightMatrixs_.lightView = Matrix4x4::LookAt(eye, Ptarget, up);
}



//void DirectionalLight::BuildMatrices(const Vector3& center, float radius)
//{
//    // ライト用の行列作成
//    Vector3 direction = lightData_->direction.Normalize();
//
//    // ライトの位置
//    Vector3 eye = center - direction * radius;
//    Vector3 up = std::abs(direction.y) > 0.99f ? Vector3::ExprUnitZ : Vector3::ExprUnitY;
//
//    // ビュー行列
//    lightMatrixs_.lightView = Matrix4x4::LookAt(eye, center, up);
//    lightMatrixs_.lightProj = Matrix4x4::Orthographic(
//        -radius, radius,
//        radius, -radius,
//        0.1f, radius * radiusScale_
//    );
//
//    // ビュー×プロジェクション行列
//    lightMatrixs_.lightVP = (lightMatrixs_.lightView * lightMatrixs_.lightProj).Transpose();
//}
