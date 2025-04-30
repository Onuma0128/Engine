#include "InputLayoutFactory.h"

#include <cassert>
#include <memory>

D3D12_INPUT_LAYOUT_DESC& InputLayoutFactory::GetInputLayout(PipelineType type)
{
	static std::unique_ptr<InputLayoutBase> inputLayout = nullptr;

	switch (type) {
	case PipelineType::Object3d:
		inputLayout = std::make_unique<Object3dInputLayout>();
		break;
	case PipelineType::Sprite:
		inputLayout = std::make_unique<SpriteInputLayout>();
		break;
	case PipelineType::Line3d:
		inputLayout = std::make_unique<Line3dInputLayout>();
		break;
	case PipelineType::Particle:
		inputLayout = std::make_unique<ParticleInputLayout>();
		break;
	case PipelineType::PrimitiveDrawr:
		inputLayout = std::make_unique<PrimitiveDrawrInputLayout>();
		break;
	case PipelineType::Animation:
		inputLayout = std::make_unique<AnimationInputLayout>();
		break;
	case PipelineType::RenderTexture:
		inputLayout = std::make_unique<RenderTextureInputLayout>();
		break;
	default:
		assert(false && "Invalid InputLayoutType");
		break;
	}

	return inputLayout->BuildLayout();
}
