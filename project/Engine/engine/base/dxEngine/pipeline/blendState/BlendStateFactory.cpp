#include "BlendStateFactory.h"

#include <cassert>
#include <memory>

D3D12_BLEND_DESC& BlendStateFactory::GetBlendState(BlendMode type)
{
    static std::unique_ptr<BlendStateBase> blendState = nullptr;

	switch (type)
	{
	case BlendMode::kBlendModeNone:
		blendState = std::make_unique<BlendStateNone>();
		break;
	case BlendMode::kBlendModeNormal:
		blendState = std::make_unique<BlendStateNormal>();
		break;
	case BlendMode::kBlendModeAdd:
		blendState = std::make_unique<BlendStateAdd>();
		break;
	case BlendMode::kBlendModeSubtract:
		blendState = std::make_unique<BlendStateSubtract>();
		break;
	case BlendMode::kBlendModeMultily:
		blendState = std::make_unique<BlendStateMultily>();
		break;
	default:
		assert(false && "Invalid BlendMode");
		break;
	}

	return blendState->BuildBlend();
}
