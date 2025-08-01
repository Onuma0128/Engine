#pragma once
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include <wrl.h>

using Microsoft::WRL::ComPtr;

ComPtr<ID3D12Resource> CreateBufferResource(ComPtr<ID3D12Device> device, size_t sizeInBytes);

ComPtr<ID3D12Resource> CreateUAVBufferResource(ComPtr<ID3D12Device> device, size_t sizeInBytes);