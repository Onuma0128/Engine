#include "SpriteBase.h"

#include "DirectXEngine.h"
#include "PipelineState.h"

void NumaEngine::SpriteBase::Initialize()
{
    rootSignature_ = NumaEngine::DirectXEngine::GetPipelineState()->GetRootSignature(PipelineType::kSprite).Get();
	pipelineState_ = NumaEngine::DirectXEngine::GetPipelineState()->GetPipelineState(PipelineType::kSprite).Get();
}

void NumaEngine::SpriteBase::NoiseInitialize()
{
    rootSignature_ = NumaEngine::DirectXEngine::GetPipelineState()->GetRootSignature(PipelineType::kSpriteDissolve).Get();
	pipelineState_ = NumaEngine::DirectXEngine::GetPipelineState()->GetPipelineState(PipelineType::kSpriteDissolve).Get();
}

void NumaEngine::SpriteBase::DrawBase()
{
    auto commandList = NumaEngine::DirectXEngine::GetCommandList();
	commandList->SetGraphicsRootSignature(rootSignature_.Get());
	commandList->SetPipelineState(pipelineState_.Get());
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

