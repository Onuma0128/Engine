#include "RootSignatureFactory.h"

#include <cassert>
#include <memory>
#include <unordered_map>

#include "Object3dRootSignature.h"
#include "SpriteRootSignature.h"
#include "Line3dRootSignature.h"
#include "ParticleRootSignature.h"
#include "PrimitiveDrawrRootSignature.h"
#include "AnimationRootSignature.h"
#include "RenderTextureRootSignature.h"
#include "SkyboxRootSignature.h"
#include "OutLineMaskRootSignature.h"
#include "AnimationOutLineMaskRootSignature.h"
#include "ObjectShadowMapDepthRootSignature.h"
#include "AnimationShadowMapDepthRootSignature.h"

ComPtr<ID3D12RootSignature> RootSignatureFactory::GetRootSignature(PipelineType type, ComPtr<ID3D12Device> device, PostEffectType effectType)
{
	static std::unordered_map<PipelineType, std::unique_ptr<RootSignatureBase>> rootSignature;

	switch (type) {
	case PipelineType::Object3d:
		rootSignature[type] = std::make_unique<Object3dRootSignature>();
		break;
	case PipelineType::Sprite:
		rootSignature[type] = std::make_unique<SpriteRootSignature>();
		break;
	case PipelineType::Line3d:
		rootSignature[type] = std::make_unique<Line3dRootSignature>();
		break;
	case PipelineType::Particle:
		rootSignature[type] = std::make_unique<ParticleRootSignature>();
		break;
	case PipelineType::PrimitiveDrawr:
		rootSignature[type] = std::make_unique<PrimitiveDrawrRootSignature>();
		break;
	case PipelineType::Animation:
		rootSignature[type] = std::make_unique<AnimationRootSignature>();
		break;
	case PipelineType::RenderTexture:
		rootSignature[type] = std::make_unique<RenderTextureRootSignature>();
		break;
	case PipelineType::Skybox:
		rootSignature[type] = std::make_unique<SkyboxRootSignature>();
		break;
	case PipelineType::ObjectOutLineMask:
		rootSignature[type] = std::make_unique<OutLineMaskRootSignature>();
		break;
	case PipelineType::AnimationOutLineMask:
		rootSignature[type] = std::make_unique<AnimationOutLineMaskRootSignature>();
		break;
	case PipelineType::ObjectShadowMapDepth:
		rootSignature[type] = std::make_unique<ObjectShadowMapDepthRootSignature>();
		break;
	case PipelineType::AnimationShadowMapDepth:
		rootSignature[type] = std::make_unique<AnimationShadowMapDepthRootSignature>();
		break;
	default:
		assert(false && "Invalid RootSignatureType");
		break;
	}

	return std::move(rootSignature[type]->BuildRootSignature(device.Get(), effectType));
}
