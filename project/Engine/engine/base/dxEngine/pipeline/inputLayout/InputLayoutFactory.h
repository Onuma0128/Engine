#pragma once

#include "InputLayoutBase.h"
#include "PipelineStruct.h"

class InputLayoutFactory
{
public:

	static D3D12_INPUT_LAYOUT_DESC& GetInputLayout(PipelineType type);

private:

};

// ============================================================
// Object3d
// ============================================================

class Object3dInputLayout : public InputLayoutBase
{
public:

	D3D12_INPUT_LAYOUT_DESC& BuildLayout() override
	{
		descs[0].SemanticName = "POSITION";
		descs[0].SemanticIndex = 0;
		descs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		descs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
		descs[1].SemanticName = "TEXCOORD";
		descs[1].SemanticIndex = 0;
		descs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
		descs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
		descs[2].SemanticName = "NORMAL";
		descs[2].SemanticIndex = 0;
		descs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		descs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

		layoutDesc.pInputElementDescs = descs.data();
		layoutDesc.NumElements = 3;

		return layoutDesc;
	}
};

// ============================================================
// Sprite
// ============================================================

class SpriteInputLayout : public InputLayoutBase
{
public:

	D3D12_INPUT_LAYOUT_DESC& BuildLayout() override
	{
		descs[0].SemanticName = "POSITION";
		descs[0].SemanticIndex = 0;
		descs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		descs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
		descs[1].SemanticName = "TEXCOORD";
		descs[1].SemanticIndex = 0;
		descs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
		descs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
		descs[2].SemanticName = "NORMAL";
		descs[2].SemanticIndex = 0;
		descs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		descs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

		layoutDesc.pInputElementDescs = descs.data();
		layoutDesc.NumElements = 3;

		return layoutDesc;
	}
};

// ============================================================
// Line3d
// ============================================================

class Line3dInputLayout : public InputLayoutBase
{
public:

	D3D12_INPUT_LAYOUT_DESC& BuildLayout() override
	{
		descs[0].SemanticName = "POSITION";
		descs[0].SemanticIndex = 0;
		descs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		descs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

		layoutDesc.pInputElementDescs = descs.data();
		layoutDesc.NumElements = 1;

		return layoutDesc;
	}
};

// ============================================================
// Particle
// ============================================================

class ParticleInputLayout : public InputLayoutBase
{
public:
	D3D12_INPUT_LAYOUT_DESC& BuildLayout() override
	{
		descs[0].SemanticName = "POSITION";
		descs[0].SemanticIndex = 0;
		descs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		descs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
		descs[1].SemanticName = "TEXCOORD";
		descs[1].SemanticIndex = 0;
		descs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
		descs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
		descs[2].SemanticName = "COLOR";
		descs[2].SemanticIndex = 0;
		descs[2].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		descs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

		layoutDesc.pInputElementDescs = descs.data();
		layoutDesc.NumElements = 3;

		return layoutDesc;
	};
};

// ============================================================
// PrimitiveDrawr
// ============================================================

class PrimitiveDrawrInputLayout : public InputLayoutBase
{
public:

	D3D12_INPUT_LAYOUT_DESC& BuildLayout() override
	{
		descs[0].SemanticName = "POSITION";
		descs[0].SemanticIndex = 0;
		descs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		descs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
		descs[1].SemanticName = "TEXCOORD";
		descs[1].SemanticIndex = 0;
		descs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
		descs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

		layoutDesc.pInputElementDescs = descs.data();
		layoutDesc.NumElements = 2;

		return layoutDesc;
	}
};

// ============================================================
// RenderTexture
// ============================================================

class RenderTextureInputLayout : public InputLayoutBase
{
public:

	D3D12_INPUT_LAYOUT_DESC& BuildLayout() override
	{
		layoutDesc.pInputElementDescs = nullptr;
		layoutDesc.NumElements = 0;

		return layoutDesc;
	}
};

// ============================================================
// Animation
// ============================================================

class AnimationInputLayout : public InputLayoutBase
{
public:

	D3D12_INPUT_LAYOUT_DESC& BuildLayout() override
	{
		descs[0].SemanticName = "POSITION";
		descs[0].SemanticIndex = 0;
		descs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		descs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
		descs[1].SemanticName = "TEXCOORD";
		descs[1].SemanticIndex = 0;
		descs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
		descs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
		descs[2].SemanticName = "NORMAL";
		descs[2].SemanticIndex = 0;
		descs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		descs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

		descs[3].SemanticName = "WEIGHT";
		descs[3].SemanticIndex = 0;
		descs[3].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		descs[3].InputSlot = 1;
		descs[3].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
		descs[4].SemanticName = "INDEX";
		descs[4].SemanticIndex = 0;
		descs[4].Format = DXGI_FORMAT_R32G32B32A32_SINT;
		descs[4].InputSlot = 1;
		descs[4].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

		layoutDesc.pInputElementDescs = descs.data();
		layoutDesc.NumElements = 5;

		return layoutDesc;
	}
};
