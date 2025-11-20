#include "PrimitiveDrawrBase.h"

#include "DirectXEngine.h"
#include "PipelineState.h"

void PrimitiveDrawrBase::Initialize()
{
	rootSignature_ = DirectXEngine::GetPipelineState()->GetRootSignature(PipelineType::kPrimitiveDrawr).Get();
	for (int i = 0; i < static_cast<int>(pipelineStates_.size()); ++i) {
		pipelineStates_[i] = DirectXEngine::GetPipelineState()->GetPipelineState(
			PipelineType::kPrimitiveDrawr, PostEffectType::kNone, static_cast<BlendMode>(i)
		).Get();
	}

	skyboxRootSignature_ = DirectXEngine::GetPipelineState()->GetRootSignature(PipelineType::kSkybox).Get();
	skyboxPipelineState_ = DirectXEngine::GetPipelineState()->GetPipelineState(PipelineType::kSkybox).Get();
}

void PrimitiveDrawrBase::DrawBase(int blendMode)
{
	auto commandList = DirectXEngine::GetCommandList();
	commandList->SetGraphicsRootSignature(rootSignature_.Get());
	commandList->SetPipelineState(pipelineStates_[blendMode].Get());
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void PrimitiveDrawrBase::DrawSkyboxBase()
{
	auto commandList = DirectXEngine::GetCommandList();
	commandList->SetGraphicsRootSignature(skyboxRootSignature_.Get());
	commandList->SetPipelineState(skyboxPipelineState_.Get());
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
