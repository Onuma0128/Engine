#include "DepthStencilStateFactory.h"

#include <cassert>
#include <memory>

D3D12_DEPTH_STENCIL_DESC& DepthStencilStateFactory::GetDepthStencilState(PipelineType type)
{
	static std::unique_ptr<DepthStencilStateBase> depthStencilState = nullptr;

	switch (type) {
	case PipelineType::Object3d:
		depthStencilState = std::make_unique<Object3dDepthStencilState>();
		break;
	case PipelineType::Sprite:
		depthStencilState = std::make_unique<SpriteDepthStencilState>();
		break;
	case PipelineType::Line3d:
		depthStencilState = std::make_unique<Line3dDepthStencilState>();
		break;
	case PipelineType::Particle:
		depthStencilState = std::make_unique<ParticleDepthStencilState>();
		break;
	case PipelineType::PrimitiveDrawr:
		depthStencilState = std::make_unique<PrimitiveDrawrDepthStencilState>();
		break;
	case PipelineType::Animation:
		depthStencilState = std::make_unique<AnimationDepthStencilState>();
		break;
	case PipelineType::RenderTexture:
		depthStencilState = std::make_unique<RenderTextureDepthStencilState>();
		break;
	default:
		assert(false && "Invalid DepthStencilStateType");
		break;
	}

	return depthStencilState->BuildDepthStencil();
}
