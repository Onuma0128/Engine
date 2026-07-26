#include "AnimationBase.h"

#include "DirectXEngine.h"
#include "PipelineState.h"

void AnimationBase::Initialize()
{
    rootSignature_ = NumaEngine::DirectXEngine::GetPipelineState()->GetRootSignature(PipelineType::kAnimation).Get();
	pipelineState_ = NumaEngine::DirectXEngine::GetPipelineState()->GetPipelineState(PipelineType::kAnimation).Get();
}

void AnimationBase::DrawBase()
{
    auto commandList = NumaEngine::DirectXEngine::GetCommandList();
	commandList->SetGraphicsRootSignature(rootSignature_.Get());
	commandList->SetPipelineState(pipelineState_.Get());
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
