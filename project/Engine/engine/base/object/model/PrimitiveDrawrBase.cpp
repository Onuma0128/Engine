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

	rootSignature_ = dxEngine_->GetPipelineState()->CreateRootSignature(PipelineType::PrimitiveDrawr).Get();
	for (int i = 0; i < static_cast<int>(pipelineStates_.size()); ++i) {
		pipelineStates_[i] = dxEngine_->GetPipelineState()->CreatePrimitiveDrawrPipelineState(i);
	}
}

void PrimitiveDrawrBase::DrawBase(int blendMode)
{
	auto commandList = dxEngine_->GetCommandList();
	commandList->SetGraphicsRootSignature(rootSignature_.Get());
	commandList->SetPipelineState(pipelineStates_[blendMode].Get());
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void PrimitiveDrawrBase::Finalize()
{
	delete instance_;
	instance_ = nullptr;
}