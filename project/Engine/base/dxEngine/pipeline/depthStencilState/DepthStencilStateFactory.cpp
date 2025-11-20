#include "DepthStencilStateFactory.h"

#include <cassert>
#include <memory>
#include <unordered_map>

D3D12_DEPTH_STENCIL_DESC& DepthStencilStateFactory::GetDepthStencilState(PipelineType type)
{
	static std::unordered_map<PipelineType, std::unique_ptr<DepthStencilStateBase>> depthStencilState;

	switch (type) {
	case PipelineType::kObject3d:
		depthStencilState[type] = std::make_unique<Object3dDepthStencilState>();
		break;
	case PipelineType::kSprite:
		depthStencilState[type] = std::make_unique<SpriteDepthStencilState>();
		break;
	case PipelineType::kLine3d:
		depthStencilState[type] = std::make_unique<Line3dDepthStencilState>();
		break;
	case PipelineType::kParticle:
		depthStencilState[type] = std::make_unique<ParticleDepthStencilState>();
		break;
	case PipelineType::kPrimitiveDrawr:
		depthStencilState[type] = std::make_unique<PrimitiveDrawrDepthStencilState>();
		break;
	case PipelineType::kAnimation:
		depthStencilState[type] = std::make_unique<AnimationDepthStencilState>();
		break;
	case PipelineType::kRenderTexture:
		depthStencilState[type] = std::make_unique<RenderTextureDepthStencilState>();
		break;
	case PipelineType::kSkybox:
		depthStencilState[type] = std::make_unique<SkyboxDepthStencilState>();
		break;
	case PipelineType::kObjectOutLineMask:
		depthStencilState[type] = std::make_unique<ObjectOutLineMaskDepthStencilState>();
		break;
	case PipelineType::kAnimationOutLineMask:
		depthStencilState[type] = std::make_unique<AnimationOutLineMaskDepthStencilState>();
		break;
	case PipelineType::kObjectShadowMapDepth:
		depthStencilState[type] = std::make_unique<Object3dDepthStencilState>();
		break;
	case PipelineType::kAnimationShadowMapDepth:
		depthStencilState[type] = std::make_unique<Object3dDepthStencilState>();
		break;
	default:
		assert(false && "Invalid DepthStencilStateType");
		break;
	}

	return depthStencilState[type]->BuildDepthStencil();
}
