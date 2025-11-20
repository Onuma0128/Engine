#include "RasterizerStateFactory.h"

#include <cassert>
#include <memory>
#include <unordered_map>

D3D12_RASTERIZER_DESC& RasterizerStateFactory::GetRasterizerDesc(PipelineType type)
{
	static std::unordered_map<PipelineType,std::unique_ptr<RasterizerStateBase>> rasterizerState;

	switch (type) {
	case PipelineType::kObject3d:
		rasterizerState[type] = std::make_unique<Object3dRasterizerState>();
		break;
	case PipelineType::kSprite:
		rasterizerState[type] = std::make_unique<SpriteRasterizerState>();
		break;
	case PipelineType::kLine3d:
		rasterizerState[type] = std::make_unique<Line3dRasterizerState>();
		break;
	case PipelineType::kParticle:
		rasterizerState[type] = std::make_unique<ParticleRasterizerState>();
		break;
	case PipelineType::kPrimitiveDrawr:
		rasterizerState[type] = std::make_unique<PrimitiveDrawrRasterizerState>();
		break;
	case PipelineType::kAnimation:
		rasterizerState[type] = std::make_unique<AnimationRasterizerState>();
		break;
	case PipelineType::kRenderTexture:
		rasterizerState[type] = std::make_unique<RenderTextureRasterizerState>();
		break;
	case PipelineType::kSkybox:
		rasterizerState[type] = std::make_unique<SkyboxRasterizerState>();
		break;
	case PipelineType::kObjectOutLineMask:
		rasterizerState[type] = std::make_unique<ObjectOutLineMaskRasterizerState>();
		break;
	case PipelineType::kAnimationOutLineMask:
		rasterizerState[type] = std::make_unique<AnimationOutLineMaskRasterizerState>();
		break;
	case PipelineType::kObjectShadowMapDepth:
		rasterizerState[type] = std::make_unique<ObjectShadowMapRasterizerState>();
		break;
	case PipelineType::kAnimationShadowMapDepth:
		rasterizerState[type] = std::make_unique<ObjectShadowMapRasterizerState>();
		break;
	default:
		assert(false && "Invalid RasterizerStateType");
		break;
	}

	return rasterizerState[type]->BuildRasterizer();
}
