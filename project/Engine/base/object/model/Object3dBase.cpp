#include "Object3dBase.h"

#include "DirectXEngine.h"
#include "PipelineState.h"

void Object3dBase::Initialize()
{
	rootSignature_ = DirectXEngine::GetPipelineState()->GetRootSignature(PipelineType::kObject3d).Get();
	pipelineState_ = DirectXEngine::GetPipelineState()->GetPipelineState(PipelineType::kObject3d).Get();
}

void Object3dBase::DrawBase()
{
	auto commandList = DirectXEngine::GetCommandList();
	commandList->SetGraphicsRootSignature(rootSignature_.Get());
	commandList->SetPipelineState(pipelineState_.Get());
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}