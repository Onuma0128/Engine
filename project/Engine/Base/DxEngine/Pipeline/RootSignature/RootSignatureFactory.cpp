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
	case PipelineType::kObject3d:
		rootSignature[type] = std::make_unique<Object3dRootSignature>();
		break;
	case PipelineType::kSprite:
		rootSignature[type] = std::make_unique<SpriteRootSignature>();
		break;
	case PipelineType::kLine3d:
		rootSignature[type] = std::make_unique<Line3dRootSignature>();
		break;
	case PipelineType::kParticle:
		rootSignature[type] = std::make_unique<ParticleRootSignature>();
		break;
	case PipelineType::kPrimitiveDrawr:
		rootSignature[type] = std::make_unique<PrimitiveDrawrRootSignature>();
		break;
	case PipelineType::kAnimation:
		rootSignature[type] = std::make_unique<AnimationRootSignature>();
		break;
	case PipelineType::kRenderTexture:
		rootSignature[type] = std::make_unique<RenderTextureRootSignature>();
		break;
	case PipelineType::kSkybox:
		rootSignature[type] = std::make_unique<SkyboxRootSignature>();
		break;
	case PipelineType::kObjectOutLineMask:
		rootSignature[type] = std::make_unique<OutLineMaskRootSignature>();
		break;
	case PipelineType::kAnimationOutLineMask:
		rootSignature[type] = std::make_unique<AnimationOutLineMaskRootSignature>();
		break;
	case PipelineType::kObjectShadowMapDepth:
		rootSignature[type] = std::make_unique<ObjectShadowMapDepthRootSignature>();
		break;
	case PipelineType::kAnimationShadowMapDepth:
		rootSignature[type] = std::make_unique<AnimationShadowMapDepthRootSignature>();
		break;
	default:
		assert(false && "Invalid RootSignatureType");
		break;
	}

	return std::move(rootSignature[type]->BuildRootSignature(device.Get(), effectType));
}
