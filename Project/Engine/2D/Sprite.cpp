#include "Sprite.h"

#include "DirectXEngine.h"

#include "CameraManager.h"
#include "SpriteBase.h"
#include "WinApp.h"
#include "TextureManager.h"
#include "SrvManager.h"
#include "LightManager.h"

#include "CreateBufferResource.h"

void Sprite::Initialize(std::string textureFilePath, bool isNoiseTexture)
{
	spriteBase_ = std::make_unique<SpriteBase>();
	if(isNoiseTexture) {
		spriteBase_->NoiseInitialize();
		isUseNoiseTexture_ = true;
		noiseTextureFilePath_ = "white1x1.png";
		noiseTextureIndex_ = TextureManager::GetInstance()->GetSrvIndex(noiseTextureFilePath_);
		DissolveDataInitialize();
	} else {
		spriteBase_->Initialize();
	}

	textureFilePath_ = textureFilePath;
	textureIndex_ = TextureManager::GetInstance()->GetSrvIndex(textureFilePath);

	VertexDataInitialize();
	MaterialDataInitialize();
	TransformationMatrixDataInitialize();
	AdjustTextureSize();
}

void Sprite::SetSceneRenderer()
{
	renderOptions_ = {
		.enabled = true,
		.offscreen = true
	};
	DirectXEngine::GetSceneRenderer()->SetDrawList(this);
}

void Sprite::Update()
{
	// セッターで貰った値を格納
	AccessorUpdate();

	UpdateMatrix();
}

void Sprite::Draw()
{
	spriteBase_->DrawBase();

	auto commandList = DirectXEngine::GetCommandList();
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
	commandList->IASetIndexBuffer(&indexBufferView_);
	commandList->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(1, transformationMatrixResource_->GetGPUVirtualAddress());
	SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(2, textureIndex_);
	if (isUseNoiseTexture_) {
		SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(3, noiseTextureIndex_);
		commandList->SetGraphicsRootConstantBufferView(4, dissolveResource_->GetGPUVirtualAddress());
	}
	
	commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

void Sprite::SetTexture(const std::string& fileName)
{
	textureFilePath_ = fileName;
	textureIndex_ = TextureManager::GetInstance()->GetSrvIndex(fileName);
	AdjustTextureSize();
}

void Sprite::SetNoiseTexture(const std::string& fileName)
{
	textureFilePath_ = fileName;
	noiseTextureIndex_ = TextureManager::GetInstance()->GetSrvIndex(fileName);
	AdjustTextureSize();
}

void Sprite::VertexDataInitialize()
{
	vertexResource_ = CreateBufferResource(DirectXEngine::GetDevice(), sizeof(VertexData) * 4);
	indexResource_ = CreateBufferResource(DirectXEngine::GetDevice(), sizeof(uint32_t) * 6);
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 4;
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * 6;
	vertexBufferView_.StrideInBytes = sizeof(VertexData);
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;
	// 書き込むためのアドレスを取得
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	// 四角形の4つの頂点
	vertexData_[0].position = { 0.0f,1.0f,0.0f,1.0f };//左下
	vertexData_[0].texcoord = { 0.0f,1.0f };
	vertexData_[1].position = { 0.0f,0.0f,0.0f,1.0f };//左上
	vertexData_[1].texcoord = { 0.0f,0.0f };
	vertexData_[2].position = { 1.0f,1.0f,0.0f,1.0f };//右下
	vertexData_[2].texcoord = { 1.0f,1.0f };
	vertexData_[3].position = { 1.0f,0.0f,0.0f,1.0f };//右上
	vertexData_[3].texcoord = { 1.0f,0.0f };
	// 法線情報の追加
	vertexData_[0].normal = { 0.0f,0.0f,-1.0f };
	// IndexData
	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));
	indexData_[0] = 0; indexData_[1] = 1; indexData_[2] = 2;
	indexData_[3] = 1; indexData_[4] = 3; indexData_[5] = 2;
}

void Sprite::MaterialDataInitialize()
{
	materialResource_ = CreateBufferResource(DirectXEngine::GetDevice(), sizeof(Material));
	
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

	materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialData_->uvTransform = Matrix4x4::Identity();
}

void Sprite::DissolveDataInitialize()
{
	dissolveResource_ = CreateBufferResource(DirectXEngine::GetDevice(), sizeof(DissolveParams));
	dissolveResource_->Map(0, nullptr, reinterpret_cast<void**>(&dissolveData_));

	dissolveData_->threshold = 0.5f;
	dissolveData_->edgeWidth = 0.1f;
	dissolveData_->edgeColor = Vector3(1.0f, 1.0f, 1.0f);
}

void Sprite::TransformationMatrixDataInitialize()
{
	transformationMatrixResource_ = CreateBufferResource(DirectXEngine::GetDevice(), sizeof(TransformationMatrix));
	transformationMatrixResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData_));
	transformationMatrixData_->WVP = Matrix4x4::Identity();
	transformationMatrixData_->World = Matrix4x4::Identity();
}

void Sprite::AccessorUpdate()
{
	// アンカーポイントを設定
	float left = 0.0f - anchorPoint_.x;
	float right = 1.0f - anchorPoint_.x;
	float top = 0.0f - anchorPoint_.y;
	float bottom = 1.0f - anchorPoint_.y;

	// フリップを設定(反転する)
	// 左右反転
	if (isFlipX_) {
		left = -left;
		right = -right;
	}
	// 上下反転
	if (isFlipY_) {
		top = -top;
		bottom = -bottom;
	}

	// アンカーポイントを設定
	vertexData_[0].position = { left,bottom,0.0f,1.0f };
	vertexData_[1].position = { left,top,0.0f,1.0f };
	vertexData_[2].position = { right,bottom,0.0f,1.0f };
	vertexData_[3].position = { right,top,0.0f,1.0f };

	const DirectX::TexMetadata& metadata = 
		TextureManager::GetInstance()->GetMetaData(textureFilePath_);
	float tex_left = textureLeftTop_.x / metadata.width;
	float tex_right = (textureLeftTop_.x + textureSize_.x) / metadata.width;
	float tex_top = textureLeftTop_.y / metadata.height;
	float tex_bottom = (textureLeftTop_.y + textureSize_.y) / metadata.height;

	vertexData_[0].texcoord = { tex_left,tex_bottom };
	vertexData_[1].texcoord = { tex_left,tex_top };
	vertexData_[2].texcoord = { tex_right,tex_bottom };
	vertexData_[3].texcoord = { tex_right,tex_top };
}

void Sprite::UpdateMatrix()
{
	Matrix4x4 worldMatrix = Matrix4x4::Affine(
		Vector3{ transform_.size.x,transform_.size.y,1.0f },
		Vector3{ 0.0f,0.0f,transform_.rotate },
		Vector3{ transform_.position.x,transform_.position.y,0.0f }
	);
	Matrix4x4 viewMatrix = Matrix4x4::Identity();
	Matrix4x4 projectionMatrix = Matrix4x4::Orthographic(0.0f, float(WinApp::kClientWidth), float(WinApp::kClientHeight), 0.0f, 0.0f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix = worldMatrix * (viewMatrix * projectionMatrix);
	transformationMatrixData_->WVP = worldViewProjectionMatrix;
	transformationMatrixData_->World = worldViewProjectionMatrix;
	transformationMatrixData_->WorldInverseTranspose = Matrix4x4::Inverse(worldViewProjectionMatrix);
}

void Sprite::AdjustTextureSize()
{
	const DirectX::TexMetadata& metadata =
		TextureManager::GetInstance()->GetMetaData(textureFilePath_);
	textureSize_ = {
		static_cast<float>(metadata.width),
		static_cast<float>(metadata.height)
	};
	transform_.size = textureSize_;
}
