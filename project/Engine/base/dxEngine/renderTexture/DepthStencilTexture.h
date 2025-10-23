#pragma once
#include "wrl.h"
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")

using Microsoft::WRL::ComPtr;

// 深度ステンシル用テクスチャリソースの生成
ComPtr<ID3D12Resource> CreateDepthStencilTextureResource(ComPtr<ID3D12Device> device, int width, int height);
