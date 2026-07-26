#include "Object3dBase.h"

#include "DirectXEngine.h"
#include "PipelineState.h"

void Object3dBase::Initialize()
{
    rootSignature_ = NumaEngine::DirectXEngine::GetPipelineState()->GetRootSignature(PipelineType::kObject3d).Get();
	pipelineState_ = NumaEngine::DirectXEngine::GetPipelineState()->GetPipelineState(PipelineType::kObject3d).Get();
}

void Object3dBase::DrawBase()
{
    auto commandList = NumaEngine::DirectXEngine::GetCommandList();
	commandList->SetGraphicsRootSignature(rootSignature_.Get());
	commandList->SetPipelineState(pipelineState_.Get());
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}