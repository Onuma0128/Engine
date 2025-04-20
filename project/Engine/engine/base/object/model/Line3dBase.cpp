#include "Line3dBase.h"

#include "DirectXEngine.h"
#include "PipelineState.h"
#include "Camera.h"

#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"

Line3dBase* Line3dBase::instance_ = nullptr;

Line3dBase* Line3dBase::GetInstance()
{
	if (instance_ == nullptr) {
		instance_ = new Line3dBase;
	}
	return instance_;
}

void Line3dBase::Initialize(DirectXEngine* dxEngine)
{
	dxEngine_ = dxEngine;

	rootSignature_ = dxEngine_->GetPipelineState()->CreateLine3dRootSignature();
	pipelineState_ = dxEngine_->GetPipelineState()->CreateLine3dPipelineState();
}

void Line3dBase::DrawBase()
{
	auto commandList = dxEngine_->GetCommandList();
	commandList->SetGraphicsRootSignature(rootSignature_.Get());
	commandList->SetPipelineState(pipelineState_.Get());
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
}

void Line3dBase::Finalize()
{
	delete instance_;
	instance_ = nullptr;
}

