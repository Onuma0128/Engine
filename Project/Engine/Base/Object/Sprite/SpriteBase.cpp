#include "SpriteBase.h"

#include "DirectXEngine.h"
#include "PipelineState.h"

void SpriteBase::Initialize()
{
	rootSignature_ = DirectXEngine::GetPipelineState()->GetRootSignature(PipelineType::kSprite).Get();
	pipelineState_ = DirectXEngine::GetPipelineState()->GetPipelineState(PipelineType::kSprite).Get();
}

void SpriteBase::NoiseInitialize()
{
	rootSignature_ = DirectXEngine::GetPipelineState()->GetRootSignature(PipelineType::kSpriteDissolve).Get();
	pipelineState_ = DirectXEngine::GetPipelineState()->GetPipelineState(PipelineType::kSpriteDissolve).Get();
}

void SpriteBase::DrawBase()
{
	auto commandList = DirectXEngine::GetCommandList();
	commandList->SetGraphicsRootSignature(rootSignature_.Get());
	commandList->SetPipelineState(pipelineState_.Get());
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
