#pragma once
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include <stdint.h>
#include "wrl.h"

using Microsoft::WRL::ComPtr;

// デスクリプタヒープ作成
ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(
	ComPtr<ID3D12Device> device, D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);
// CPUデスクリプタハンドル取得
D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(
	ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index);
// GPUデスクリプタハンドル取得
D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(
	ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index);