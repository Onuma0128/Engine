#include "RootSignatureFactory.h"

#include <cassert>
#include <memory>

#include "Object3dRootSignature.h"
#include "SpriteRootSignature.h"
#include "Line3dRootSignature.h"
#include "ParticleRootSignature.h"
#include "PrimitiveDrawrRootSignature.h"
#include "AnimationRootSignature.h"
#include "RenderTextureRootSignature.h"
#include "SkyboxRootSignature.h"

ComPtr<ID3D12RootSignature> RootSignatureFactory::GetRootSignature(PipelineType type, ComPtr<ID3D12Device> device, PostEffectType effectType)
{
	static std::unique_ptr<RootSignatureBase> rootSignature = nullptr;

	switch (type) {
	case PipelineType::Object3d:
		rootSignature = std::make_unique<Object3dRootSignature>();
		break;
	case PipelineType::Sprite:
		rootSignature = std::make_unique<SpriteRootSignature>();
		break;
	case PipelineType::Line3d:
		rootSignature = std::make_unique<Line3dRootSignature>();
		break;
	case PipelineType::Particle:
		rootSignature = std::make_unique<ParticleRootSignature>();
		break;
	case PipelineType::PrimitiveDrawr:
		rootSignature = std::make_unique<PrimitiveDrawrRootSignature>();
		break;
	case PipelineType::Animation:
		rootSignature = std::make_unique<AnimationRootSignature>();
		break;
	case PipelineType::RenderTexture:
		rootSignature = std::make_unique<RenderTextureRootSignature>();
		break;
	case PipelineType::Skybox:
		rootSignature = std::make_unique<SkyboxRootSignature>();
		break;
	default:
		assert(false && "Invalid RootSignatureType");
		break;
	}

	return rootSignature->BuildRootSignature(device.Get(), effectType);
}
