#include "TrailEffectBase.h"

#include "DirectXEngine.h"
#include "PipelineState.h"

#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"

TrailEffectBase* TrailEffectBase::instance_ = nullptr;

TrailEffectBase* TrailEffectBase::GetInstance()
{
	if (instance_ == nullptr) {
		instance_ = new TrailEffectBase;
	}
	return instance_;
}

void TrailEffectBase::Initialize(DirectXEngine* dxEngine)
{
	dxEngine_ = dxEngine;

	rootSignature_ = dxEngine_->GetPipelineState()->CreateTrailEffectRootSignature().Get();
	pipelineState_ = dxEngine_->GetPipelineState()->CreateTrailEffectPipelineState().Get();
}

void TrailEffectBase::DrawBase()
{
	auto commandList = dxEngine_->GetCommandList();
	commandList->SetGraphicsRootSignature(rootSignature_.Get());
	commandList->SetPipelineState(pipelineState_.Get());
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void TrailEffectBase::Finalize()
{
	delete instance_;
	instance_ = nullptr;
}