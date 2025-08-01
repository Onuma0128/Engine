#include "AnimationBase.h"

#include "DirectXEngine.h"
#include "PipelineState.h"

void AnimationBase::Initialize()
{
	rootSignature_ = DirectXEngine::GetPipelineState()->GetRootSignature(PipelineType::Animation).Get();
	pipelineState_ = DirectXEngine::GetPipelineState()->GetPipelineState(PipelineType::Animation).Get();

	skinningRootSignature_ = DirectXEngine::GetPipelineState()->GetRootSignature(PipelineType::Skinning).Get();
	skinningPipelineState_ = DirectXEngine::GetPipelineState()->GetPipelineState(PipelineType::Skinning).Get();

}

void AnimationBase::BindSkinningBase()
{
	auto commandList = DirectXEngine::GetCommandList();
	commandList->SetComputeRootSignature(skinningRootSignature_.Get());
	commandList->SetPipelineState(skinningPipelineState_.Get());
}

void AnimationBase::DrawBase()
{
	auto commandList = DirectXEngine::GetCommandList();
	commandList->SetGraphicsRootSignature(rootSignature_.Get());
	commandList->SetPipelineState(pipelineState_.Get());
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
