#include "Object3dBase.h"

#include "DirectXEngine.h"
#include "PipelineState.h"

Object3dBase* Object3dBase::instance_ = nullptr;

Object3dBase* Object3dBase::GetInstance()
{
	if (instance_ == nullptr) {
		instance_ = new Object3dBase;
	}
	return instance_;
}

void Object3dBase::Initialize(DirectXEngine* dxEngine)
{
	dxEngine_ = dxEngine;

	rootSignature_ = dxEngine_->GetPipelineState()->CreateRootSignature(PipelineType::Object3d).Get();
	pipelineState_ = dxEngine_->GetPipelineState()->CreateObject3dPipelineState().Get();
}

void Object3dBase::DrawBase()
{
	auto commandList = dxEngine_->GetCommandList();
	commandList->SetGraphicsRootSignature(rootSignature_.Get());
	commandList->SetPipelineState(pipelineState_.Get());
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Object3dBase::Finalize()
{
	delete instance_;
	instance_ = nullptr;
}
