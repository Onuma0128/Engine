#include "SpriteBase.h"

#include "DirectXEngine.h"
#include "PipelineState.h"

SpriteBase* SpriteBase::instance_ = nullptr;

SpriteBase* SpriteBase::GetInstance()
{
	if (instance_ == nullptr) {
		instance_ = new SpriteBase;
	}
	return instance_;
}

void SpriteBase::Initialize(DirectXEngine* dxEngine)
{
	dxEngine_ = dxEngine;

	rootSignature_ = dxEngine_->GetPipelineState()->CreateRootSignature(PipelineType::Sprite).Get();
	pipelineState_ = dxEngine_->GetPipelineState()->CreateSpritePipelineState().Get();
}

void SpriteBase::DrawBase()
{
	auto commandList = dxEngine_->GetCommandList();
	commandList->SetGraphicsRootSignature(rootSignature_.Get());
	commandList->SetPipelineState(pipelineState_.Get());
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void SpriteBase::Finalize()
{
	delete instance_;
	instance_ = nullptr;
}
