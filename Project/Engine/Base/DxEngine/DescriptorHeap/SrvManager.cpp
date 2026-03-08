#include "SrvManager.h"

#include <cassert>
#include <stdexcept>

#include "DirectXEngine.h"
#include "DescriptorHeap.h"
#include "TextureManager.h"

const uint32_t SrvManager::kMaxSRVCount_ = 2048;

std::unique_ptr<SrvManager> SrvManager::instance_ = nullptr;

namespace
{
	// 動的に使えるSRV数
	// 先頭は TextureManager 側が使う前提なので、その分を除く
	uint32_t GetDynamicSrvCapacity()
	{
		assert(SrvManager::kMaxSRVCount_ > TextureManager::kSRVIndexTop);
		return SrvManager::kMaxSRVCount_ - TextureManager::kSRVIndexTop;
	}
}

SrvManager* SrvManager::GetInstance()
{
	if (instance_ == nullptr) {
		instance_ = std::make_unique<SrvManager>();
	}
	return instance_.get();
}

void SrvManager::Initialize(DirectXEngine* dxEngine)
{
	dxEngine_ = dxEngine;

	// デスクリプタヒープの生成
	descriptorHeap_ = CreateDescriptorHeap(
		dxEngine_->GetDevice(),
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
		kMaxSRVCount_,
		true);

	// デスクリプタ1個分のサイズを取得して記録
	descriptorSize_ = dxEngine_->GetDevice()->GetDescriptorHandleIncrementSize(
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	useIndex_ = 0;

	while (!freeIndices_.empty()) {
		freeIndices_.pop();
	}

	isAllocated_.assign(GetDynamicSrvCapacity(), false);
}

void SrvManager::SetGraphicsRootDescriptorTable(UINT RootParameterIndex, uint32_t srvIndex)
{
	dxEngine_->GetCommandList()->SetGraphicsRootDescriptorTable(
		RootParameterIndex,
		GetGPUDescriptorHandle(srvIndex));
}

void SrvManager::PreDraw()
{
	ID3D12DescriptorHeap* descriptorHeaps[] = { descriptorHeap_.Get() };
	dxEngine_->GetCommandList()->SetDescriptorHeaps(1, descriptorHeaps);
}

void SrvManager::Finalize()
{
	while (!freeIndices_.empty()) {
		freeIndices_.pop();
	}
	isAllocated_.clear();
	useIndex_ = 0;
	instance_ = nullptr;
}

D3D12_CPU_DESCRIPTOR_HANDLE SrvManager::GetCPUDescriptorHandle(uint32_t index)
{
	assert(index < kMaxSRVCount_);

	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize_ * index);
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE SrvManager::GetGPUDescriptorHandle(uint32_t index)
{
	assert(index < kMaxSRVCount_);

	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap_->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize_ * index);
	return handleGPU;
}

void SrvManager::CreateSRVforTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT Format, UINT MipLevels)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = UINT(MipLevels);

	dxEngine_->GetDevice()->CreateShaderResourceView(
		pResource, &srvDesc, GetCPUDescriptorHandle(srvIndex));
}

void SrvManager::CreateSRVforStructuredBuffer(uint32_t srvIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	srvDesc.Buffer.NumElements = numElements;
	srvDesc.Buffer.StructureByteStride = structureByteStride;

	dxEngine_->GetDevice()->CreateShaderResourceView(
		pResource, &srvDesc, GetCPUDescriptorHandle(srvIndex));
}

void SrvManager::CreateSRVforRenderTexture(uint32_t srvIndex, ID3D12Resource* pResource, PostEffectType type)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	if (type == PostEffectType::kOutLine) {
		srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	} else {
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	}
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	dxEngine_->GetDevice()->CreateShaderResourceView(
		pResource, &srvDesc, GetCPUDescriptorHandle(srvIndex));
}

void SrvManager::CreateSRVforTextureCube(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT Format, UINT MipLevels)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
	srvDesc.TextureCube.MostDetailedMip = 0;
	srvDesc.TextureCube.MipLevels = UINT(MipLevels);
	srvDesc.TextureCube.ResourceMinLODClamp = 0.0f;

	dxEngine_->GetDevice()->CreateShaderResourceView(
		pResource, &srvDesc, GetCPUDescriptorHandle(srvIndex));
}

void SrvManager::CreateSRVforDepth(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT Format)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	dxEngine_->GetDevice()->CreateShaderResourceView(
		pResource, &srvDesc, GetCPUDescriptorHandle(srvIndex));
}

uint32_t SrvManager::Allocate()
{
	const uint32_t dynamicCapacity = GetDynamicSrvCapacity();

	// 解放済みindexの再利用
	if (!freeIndices_.empty()) {
		uint32_t index = freeIndices_.front();
		freeIndices_.pop();

		assert(index < dynamicCapacity);
		assert(isAllocated_[index] == false);

		isAllocated_[index] = true;
		return index;
	}

	// 新規割り当て
	if (useIndex_ >= dynamicCapacity) {
		throw std::runtime_error("SrvManager::Allocate() : SRV heap is full.");
	}

	uint32_t index = useIndex_;
	useIndex_++;

	isAllocated_[index] = true;
	return index;
}

void SrvManager::Free(uint32_t srvIndex)
{
	// 先頭の予約領域より前は解放対象外
	if (srvIndex < TextureManager::kSRVIndexTop) {
		return;
	}

	uint32_t localIndex = srvIndex - TextureManager::kSRVIndexTop;

	if (localIndex >= isAllocated_.size()) {
		return;
	}

	// 二重解放防止
	if (!isAllocated_[localIndex]) {
		return;
	}

	isAllocated_[localIndex] = false;
	freeIndices_.push(localIndex);
}

bool SrvManager::CheckAllocate()
{
	if (!freeIndices_.empty()) {
		return true;
	}

	return useIndex_ < GetDynamicSrvCapacity();
}