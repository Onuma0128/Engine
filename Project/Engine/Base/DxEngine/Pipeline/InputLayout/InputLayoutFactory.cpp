#include "InputLayoutFactory.h"

#include <cassert>
#include <memory>
#include <unordered_map>

D3D12_INPUT_LAYOUT_DESC& InputLayoutFactory::GetInputLayout(PipelineType type)
{
	static std::unordered_map<PipelineType, std::unique_ptr<InputLayoutBase>> inputLayout;

	switch (type) {
	case PipelineType::kObject3d:
		inputLayout[type] = std::make_unique<Object3dInputLayout>();
		break;
	case PipelineType::kSprite:
		inputLayout[type] = std::make_unique<SpriteInputLayout>();
		break;
	case PipelineType::kLine3d:
		inputLayout[type] = std::make_unique<Line3dInputLayout>();
		break;
	case PipelineType::kParticle:
		inputLayout[type] = std::make_unique<ParticleInputLayout>();
		break;
	case PipelineType::kPrimitiveDrawr:
		inputLayout[type] = std::make_unique<PrimitiveDrawrInputLayout>();
		break;
	case PipelineType::kAnimation:
		inputLayout[type] = std::make_unique<AnimationInputLayout>();
		break;
	case PipelineType::kRenderTexture:
		inputLayout[type] = std::make_unique<RenderTextureInputLayout>();
		break;
	case PipelineType::kSkybox:
		inputLayout[type] = std::make_unique<SkyboxInputLayout>();
		break;
	case PipelineType::kObjectOutLineMask:
		inputLayout[type] = std::make_unique<ObjectOutLineMaskInputLayout>();
		break;
	case PipelineType::kAnimationOutLineMask:
		inputLayout[type] = std::make_unique<AnimationOutLineMaskInputLayout>();
		break;
	case PipelineType::kObjectShadowMapDepth:
		inputLayout[type] = std::make_unique<Object3dInputLayout>();
		break;
	case PipelineType::kAnimationShadowMapDepth:
		inputLayout[type] = std::make_unique<AnimationInputLayout>();
		break;
	case PipelineType::kSpriteDissolve:
		inputLayout[type] = std::make_unique<SpriteInputLayout>();
		break;
	default:
		assert(false && "Invalid InputLayoutType");
		break;
	}

	return inputLayout[type]->BuildLayout();
}
