#pragma once
#include "wrl.h"
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include <stdint.h>

#include "Vector4.h"

using Microsoft::WRL::ComPtr;

ComPtr<ID3D12Resource> CreateRenderTextureResource(ComPtr<ID3D12Device> device, uint32_t width, uint32_t height,
	DXGI_FORMAT format, const Vector4& clearColor);