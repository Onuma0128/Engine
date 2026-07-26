#include "Line3dBase.h"

#include "DirectXEngine.h"
#include "PipelineState.h"

void Line3dBase::Initialize()
{
    rootSignature_ = NumaEngine::DirectXEngine::GetPipelineState()->GetRootSignature(PipelineType::kLine3d).Get();
	pipelineState_ = NumaEngine::DirectXEngine::GetPipelineState()->GetPipelineState(PipelineType::kLine3d).Get();
}

void Line3dBase::DrawBase()
{
    auto commandList = NumaEngine::DirectXEngine::GetCommandList();
	commandList->SetGraphicsRootSignature(rootSignature_.Get());
	commandList->SetPipelineState(pipelineState_.Get());
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
}
