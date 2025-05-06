#include "Object3dBase.h"

#include "DirectXEngine.h"
#include "PipelineState.h"

void Object3dBase::Initialize()
{
	rootSignature_ = DirectXEngine::GetPipelineState()->CreateRootSignature(PipelineType::Object3d);
	pipelineState_ = DirectXEngine::GetPipelineState()->CreateObject3dPipelineState();
}

void Object3dBase::DrawBase()
{
	auto commandList = DirectXEngine::GetCommandList();
	commandList->SetGraphicsRootSignature(rootSignature_.Get());
	commandList->SetPipelineState(pipelineState_.Get());
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}