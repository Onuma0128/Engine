#include "PrimitiveDrawrBase.h"

#include "DirectXEngine.h"
#include "PipelineState.h"

#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"

PrimitiveDrawrBase* PrimitiveDrawrBase::instance_ = nullptr;

PrimitiveDrawrBase* PrimitiveDrawrBase::GetInstance()
{
	if (instance_ == nullptr) {
		instance_ = new PrimitiveDrawrBase;
	}
	return instance_;
}

void PrimitiveDrawrBase::Initialize(DirectXEngine* dxEngine)
{
	dxEngine_ = dxEngine;

	rootSignature_ = dxEngine_->GetPipelineState()->CreateTrailEffectRootSignature().Get();
	pipelineState_ = dxEngine_->GetPipelineState()->CreateTrailEffectPipelineState().Get();
}

void PrimitiveDrawrBase::DrawBase()
{
	auto commandList = dxEngine_->GetCommandList();
	commandList->SetGraphicsRootSignature(rootSignature_.Get());
	commandList->SetPipelineState(pipelineState_.Get());
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void PrimitiveDrawrBase::Finalize()
{
	delete instance_;
	instance_ = nullptr;
}