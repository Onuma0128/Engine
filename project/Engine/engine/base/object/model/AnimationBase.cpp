#include "AnimationBase.h"

#include "DirectXEngine.h"
#include "PipelineState.h"

AnimationBase* AnimationBase::instance_ = nullptr;

AnimationBase* AnimationBase::GetInstance()
{
	if (instance_ == nullptr) {
		instance_ = new AnimationBase;
	}
	return instance_;
}

void AnimationBase::Initialize(DirectXEngine* dxEngine)
{
	dxEngine_ = dxEngine;

	rootSignature_ = dxEngine_->GetPipelineState()->CreateRootSignature(PipelineType::Animation).Get();
	pipelineState_ = dxEngine_->GetPipelineState()->CreateAnimationPipelineState().Get();
}

void AnimationBase::DrawBase()
{
	auto commandList = dxEngine_->GetCommandList();
	commandList->SetGraphicsRootSignature(rootSignature_.Get());
	commandList->SetPipelineState(pipelineState_.Get());
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void AnimationBase::Finalize()
{
	delete instance_;
	instance_ = nullptr;
}
