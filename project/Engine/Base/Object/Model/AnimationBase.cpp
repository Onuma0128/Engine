#include "AnimationBase.h"

#include "DirectXEngine.h"
#include "PipelineState.h"

void AnimationBase::Initialize()
{
	rootSignature_ = DirectXEngine::GetPipelineState()->GetRootSignature(PipelineType::kAnimation).Get();
	pipelineState_ = DirectXEngine::GetPipelineState()->GetPipelineState(PipelineType::kAnimation).Get();
}

void AnimationBase::DrawBase()
{
	auto commandList = DirectXEngine::GetCommandList();
	commandList->SetGraphicsRootSignature(rootSignature_.Get());
	commandList->SetPipelineState(pipelineState_.Get());
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
