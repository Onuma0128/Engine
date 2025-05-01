#include "RasterizerStateFactory.h"

#include <cassert>
#include <memory>

D3D12_RASTERIZER_DESC& RasterizerStateFactory::GetRasterizerDesc(PipelineType type)
{
	static std::unique_ptr<RasterizerStateBase> rasterizerState = nullptr;

	switch (type) {
	case PipelineType::Object3d:
		rasterizerState = std::make_unique<Object3dRasterizerState>();
		break;
	case PipelineType::Sprite:
		rasterizerState = std::make_unique<SpriteRasterizerState>();
		break;
	case PipelineType::Line3d:
		rasterizerState = std::make_unique<Line3dRasterizerState>();
		break;
	case PipelineType::Particle:
		rasterizerState = std::make_unique<ParticleRasterizerState>();
		break;
	case PipelineType::PrimitiveDrawr:
		rasterizerState = std::make_unique<PrimitiveDrawrRasterizerState>();
		break;
	case PipelineType::Animation:
		rasterizerState = std::make_unique<AnimationRasterizerState>();
		break;
	case PipelineType::RenderTexture:
		rasterizerState = std::make_unique<RenderTextureRasterizerState>();
		break;
	case PipelineType::Skybox:
		rasterizerState = std::make_unique<SkyboxRasterizerState>();
		break;
	default:
		assert(false && "Invalid RasterizerStateType");
		break;
	}

	return rasterizerState->BuildRasterizer();
}
