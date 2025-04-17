#include "TrailEffect.h"

#include <numbers>

#include "DirectXEngine.h"
#include "TrailEffectBase.h"
#include "TextureManager.h"
#include "SrvManager.h"

#include "Camera.h"
#include "CameraManager.h"

void TrailEffect::Init(std::vector<Vector3> pos)
{
	trailEffectBase_ = TrailEffectBase::GetInstance();
	positions_ = pos;

	SetTexture("resources", "white1x1.png");

	CreateBufferResource(vertexResource_, sizeof(VertexData) * 4);
	CreateVertexBufferView(4);
	CreateVertexData();

	CreateBufferResource(indexResource_, sizeof(uint32_t) * 6);
	CreateIndexBufferView();
	CreateIndexData();

	CreateBufferResource(materialResource_, sizeof(MaterialData));
	CreateMaterialData();

	CreateBufferResource(wvpResource_, sizeof(Matrix4x4));
	CreateWVPData();
}

void TrailEffect::Update()
{
	Matrix4x4 matWorld = Matrix4x4::Affine(transform_.scale, transform_.rotation, transform_.translation);

	*wvpData_ = matWorld * CameraManager::GetInstance()->GetActiveCamera()->GetViewProjectionMatrix();
}

void TrailEffect::Draw()
{
	trailEffectBase_->DrawBase();

	auto commandList = trailEffectBase_->GetDxEngine()->GetCommandList();
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
	commandList->IASetIndexBuffer(&indexBufferView_);
	commandList->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());
	SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(2, textureData_.textureIndex);

	commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

void TrailEffect::TypeInit(PrimitiveType type, uint32_t kIndex)
{
	type_ = type;

	switch (type_)
	{
	case PrimitiveType::Plane:
		InitPlane();
		break;
	case PrimitiveType::Sphere:
		InitSphere(kIndex);
		break;
	case PrimitiveType::Ring:
		InitRing(kIndex);
		break;
	default:
		break;
	}
}

void TrailEffect::TypeDraw()
{
	switch (type_)
	{
	case PrimitiveType::Plane:
		DrawPlane();
		break;
	case PrimitiveType::Sphere:
		DrawSphere();
		break;
	case PrimitiveType::Ring:
		DrawRing();
		break;
	default:
		break;
	}
}

void TrailEffect::SetPosition(std::vector<Vector3> pos)
{
	vertexData_[0].position = { pos[0].x,pos[0].y,pos[0].z,1.0f };
	vertexData_[1].position = { pos[1].x,pos[1].y,pos[1].z,1.0f };
	vertexData_[2].position = { pos[2].x,pos[2].y,pos[2].z,1.0f };
	vertexData_[3].position = { pos[3].x,pos[3].y,pos[3].z,1.0f };
}

void TrailEffect::SetTexture(const std::string& directoryPath, const std::string& filePath)
{
	textureData_.directoryPath = directoryPath;
	textureData_.filePath = filePath;

	TextureManager::GetInstance()->LoadTexture(textureData_.directoryPath + "/" + textureData_.filePath);
	textureData_.textureIndex = TextureManager::GetInstance()->GetSrvIndex(textureData_.directoryPath + "/" + textureData_.filePath);
}

void TrailEffect::CreateBufferResource(ComPtr<ID3D12Resource>& resource, size_t size)
{
	// 頂点リソースのヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties = {};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;

	// 頂点リソースの設定
	D3D12_RESOURCE_DESC vertexResourceDesc = {};
	vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	vertexResourceDesc.Width = size;
	vertexResourceDesc.Height = 1;
	vertexResourceDesc.DepthOrArraySize = 1;
	vertexResourceDesc.MipLevels = 1;
	vertexResourceDesc.SampleDesc.Count = 1;
	vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 頂点リソースを作成する
	HRESULT hr = trailEffectBase_->GetDxEngine()->GetDevice()->CreateCommittedResource(
		&uploadHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&vertexResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&resource));

	assert(SUCCEEDED(hr)); // エラーチェック
}

void TrailEffect::CreateVertexBufferView(uint32_t kVertexSize)
{
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * kVertexSize;
	vertexBufferView_.StrideInBytes = sizeof(VertexData);
}

void TrailEffect::CreateIndexBufferView()
{
	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * 6;
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;
}

void TrailEffect::CreateVertexData()
{
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	vertexData_[0].position = { positions_[0].x,positions_[0].y,positions_[0].z,1.0f };
	vertexData_[1].position = { positions_[1].x,positions_[1].y,positions_[1].z,1.0f };
	vertexData_[2].position = { positions_[2].x,positions_[2].y,positions_[2].z,1.0f };
	vertexData_[3].position = { positions_[3].x,positions_[3].y,positions_[3].z,1.0f };

	vertexData_[0].texcoord = { 0.0f,0.0f };
	vertexData_[1].texcoord = { 1.0f,0.0f };
	vertexData_[2].texcoord = { 0.0f,1.0f };
	vertexData_[3].texcoord = { 1.0f,1.0f };

	/*vertexData_[0] = { -0.5f,-0.5f,0.0f,1.0f };*/
	/*vertexData_[1] = { -0.5f,0.5f,0.0f,1.0f };*/
	/*vertexData_[2] = { 0.5f,-0.5f,0.0f,1.0f };*/
	/*vertexData_[3] = { 0.5f,0.5f,0.0f,1.0f };*/
}

void TrailEffect::CreateIndexData()
{
	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));

	indexData_[0] = 0; indexData_[1] = 1; indexData_[2] = 2;
	indexData_[3] = 1; indexData_[4] = 3; indexData_[5] = 2;
}

void TrailEffect::CreateMaterialData()
{
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

	materialData_->color = { 1.0f,1.0f,1.0f,1.0f };
	materialData_->xTexcoord_alpha = false;
	materialData_->yTexcoord_alpha = false;
}

void TrailEffect::CreateWVPData()
{
	wvpResource_->Map(0, nullptr, reinterpret_cast<void**>(&wvpData_));

	*wvpData_ = Matrix4x4::Identity();
}

void TrailEffect::InitPlane()
{
	trailEffectBase_ = TrailEffectBase::GetInstance();

	SetTexture("resources", "uvChecker.png");

	CreateBufferResource(vertexResource_, sizeof(VertexData) * 6);
	CreateVertexBufferView(6);
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	CreatePlaneVertexData(vertexData_);

	CreateBufferResource(materialResource_, sizeof(MaterialData));
	CreateMaterialData();

	CreateBufferResource(wvpResource_, sizeof(Matrix4x4));
	CreateWVPData();
}

void TrailEffect::DrawPlane()
{
	trailEffectBase_->DrawBase();

	auto commandList = trailEffectBase_->GetDxEngine()->GetCommandList();
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
	commandList->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());
	SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(2, textureData_.textureIndex);

	commandList->DrawInstanced(6, 1, 0, 0);
}

void TrailEffect::CreatePlaneVertexData(VertexData* vertexData)
{
	// 1,3,2
	vertexData[0] = {
		.position = { -1.0f,1.0f,0.0f,1.0f },
		.texcoord = { 0.0f,0.0f }
	};
	vertexData[1] = {
		.position = { -1.0f,-1.0f,0.0f,1.0f },
		.texcoord = { 0.0f,1.0f }
	};
	vertexData[2] = {
		.position = { 1.0f,1.0f,0.0f,1.0f },
		.texcoord = { 1.0f,0.0f }
	};

	// 2,3,4
	vertexData[3] = vertexData[2];
	vertexData[4] = vertexData[1];
	vertexData[5] = {
		.position = { 1.0f,-1.0f,0.0f,1.0f },
		.texcoord = { 1.0f,1.0f }
	};
}

void TrailEffect::InitSphere(uint32_t kSubdivision)
{
	kSubdivision_ = kSubdivision;
	trailEffectBase_ = TrailEffectBase::GetInstance();

	SetTexture("resources", "white1x1.png");

	uint32_t startIndex = kSubdivision * kSubdivision * 6;
	CreateBufferResource(vertexResource_, sizeof(VertexData) * startIndex);
	CreateVertexBufferView(startIndex);
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	CreateSphereVertexData(vertexData_, kSubdivision);

	CreateBufferResource(materialResource_, sizeof(MaterialData));
	CreateMaterialData();

	CreateBufferResource(wvpResource_, sizeof(Matrix4x4));
	CreateWVPData();
}

void TrailEffect::DrawSphere()
{
	trailEffectBase_->DrawBase();

	auto commandList = trailEffectBase_->GetDxEngine()->GetCommandList();
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
	commandList->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());
	SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(2, textureData_.textureIndex);

	commandList->DrawInstanced(kSubdivision_ * kSubdivision_ * 6, 1, 0, 0);
}

void TrailEffect::CreateSphereVertexData(VertexData* vertexData, uint32_t kSubdivision)
{
	const float pi = static_cast<float>(std::numbers::pi);
	const float kLonEvery = 2 * pi / float(kSubdivision); // 経度
	const float kLatEvery = pi / float(kSubdivision);     // 緯度

	for (uint32_t latIndex = 0; latIndex < kSubdivision; latIndex++) {
		float lat0 = -pi / 2.0f + kLatEvery * latIndex; // 緯度の方向に分割
		float lat1 = lat0 + kLatEvery;
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; lonIndex++) {
			uint32_t start = (latIndex * kSubdivision + lonIndex) * 6;
			float lon0 = kLonEvery * lonIndex; // 経度の方向に分割
			float lon1 = lon0 + kLonEvery;
			float u0 = float(lonIndex) / float(kSubdivision);
			float u1 = float(lonIndex + 1) / float(kSubdivision);
			float v0 = 1.0f - float(latIndex) / float(kSubdivision);
			float v1 = 1.0f - float(latIndex + 1) / float(kSubdivision);
			//頂点座標を計算
			vertexData[start].position = { std::cos(lat0) * std::cos(lon0), std::sin(lat0), std::cos(lat0) * std::sin(lon0), 1.0f };
			vertexData[start].texcoord = { u0, v0 };
			vertexData[start + 1].position = { std::cos(lat1) * std::cos(lon0), std::sin(lat1), std::cos(lat1) * std::sin(lon0), 1.0f };
			vertexData[start + 1].texcoord = { u0, v1 };
			vertexData[start + 2].position = { std::cos(lat0) * std::cos(lon1), std::sin(lat0), std::cos(lat0) * std::sin(lon1), 1.0f };
			vertexData[start + 2].texcoord = { u1, v0 };
			vertexData[start + 3].position = { std::cos(lat1) * std::cos(lon0), std::sin(lat1), std::cos(lat1) * std::sin(lon0), 1.0f };
			vertexData[start + 3].texcoord = { u0, v1 };
			vertexData[start + 4].position = { std::cos(lat1) * std::cos(lon1), std::sin(lat1), std::cos(lat1) * std::sin(lon1), 1.0f };
			vertexData[start + 4].texcoord = { u1, v1 };
			vertexData[start + 5].position = { std::cos(lat0) * std::cos(lon1), std::sin(lat0), std::cos(lat0) * std::sin(lon1), 1.0f };
			vertexData[start + 5].texcoord = { u1, v0 };
		}
	}
}

void TrailEffect::InitRing(uint32_t kRingDivide)
{
	kRingDivide_ = kRingDivide;
	trailEffectBase_ = TrailEffectBase::GetInstance();

	SetTexture("resources", "gradationLine.png");

	CreateBufferResource(vertexResource_, sizeof(VertexData) * kRingDivide_ * 6);
	CreateVertexBufferView(kRingDivide_ * 6);
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	CreateRingVertexData(vertexData_, kRingDivide_);

	CreateBufferResource(materialResource_, sizeof(MaterialData));
	CreateMaterialData();

	CreateBufferResource(wvpResource_, sizeof(Matrix4x4));
	CreateWVPData();
}

void TrailEffect::DrawRing()
{
	trailEffectBase_->DrawBase();

	auto commandList = trailEffectBase_->GetDxEngine()->GetCommandList();
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
	commandList->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());
	SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(2, textureData_.textureIndex);

	commandList->DrawInstanced(kRingDivide_ * 6, 1, 0, 0);
}

void TrailEffect::CreateRingVertexData(VertexData* vertexData, uint32_t kRingDivide)
{
	const float kOuterRadius = 1.0f;
	const float kInnerRadius = 0.2f;
	const float radianPerDivde = 2.0f * std::numbers::pi_v<float> / static_cast<float>(kRingDivide);

	for (uint32_t index = 0; index < kRingDivide; ++index) {
		float sin = std::sin(index * radianPerDivde);
		float cos = std::cos(index * radianPerDivde);
		uint32_t nextIndex = index + 1;
		float sinNext = std::sin(nextIndex * radianPerDivde);
		float cosNext = std::cos(nextIndex * radianPerDivde);
		float u = static_cast<float>(index) / static_cast<float>(kRingDivide);
		float uNext = static_cast<float>(nextIndex) / static_cast<float>(kRingDivide);

		uint32_t counter = index * 6;
		
		// 1,3,2
		vertexData[counter] = {
			.position = {-sin * kOuterRadius,cos * kOuterRadius,0.0f,1.0f},
			.texcoord = {u,0.0f}
		};
		vertexData[counter + 1] = {
			.position = {-sin * kInnerRadius,cos * kInnerRadius,0.0f,1.0f},
			.texcoord = {u,1.0f}
		};
		vertexData[counter + 2] = {
			.position = {-sinNext * kOuterRadius,cosNext * kOuterRadius,0.0f,1.0f},
			.texcoord = {uNext,0.0f}
		};
		// 2,3,4
		vertexData[counter + 3] = vertexData[counter + 2];

		vertexData[counter + 4] = vertexData[counter + 1];

		vertexData[counter + 5] = {
			.position = {-sinNext * kInnerRadius,cosNext * kInnerRadius,0.0f,1.0f},
			.texcoord = {uNext,1.0f}
		};
	}
}
