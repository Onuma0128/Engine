#include "Line3dBase.h"

#include "DirectXEngine.h"
#include "PipelineState.h"

void Line3dBase::Initialize()
{
	rootSignature_ = DirectXEngine::GetPipelineState()->CreateRootSignature(PipelineType::Line3d);
	pipelineState_ = DirectXEngine::GetPipelineState()->CreateLine3dPipelineState();
}

void Line3dBase::DrawBase()
{
	auto commandList = DirectXEngine::GetCommandList();
	commandList->SetGraphicsRootSignature(rootSignature_.Get());
	commandList->SetPipelineState(pipelineState_.Get());
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
}
