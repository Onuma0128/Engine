#include "Animation.h"

#include <execution>
#include <numeric>

#include "DirectXEngine.h"

#include "Object3d.h"
#include "Model.h"
#include "ModelManager.h"
#include "LightManager.h"
#include "CameraManager.h"

void Animation::Initialize(const std::string& filename)
{
	animationBase_ = std::make_unique<AnimationBase>();
	animationBase_->Initialize();

	transform_ = WorldTransform();
	SetModel(filename);
	MakeMaterialData();

	playAnim_.Init(model_->GetModelData());

	skeleton_.CreateSkeleton(transform_, model_->GetModelData().rootNode);

	skinCluster_.CreateSkinCluster(skeleton_, model_->GetModelData());
}

void Animation::SetSceneRenderer()
{
	DirectXEngine::GetModelRenderer()->Push(this);
}

void Animation::RemoveRenderer()
{
	DirectXEngine::GetModelRenderer()->Remove(this);
	/*if (line_ == nullptr) { return; }
	DirectXEngine::GetSceneRenderer()->SetRemoveList(line_.get());*/
}

void Animation::Update()
{
	if (playAnim_.GetStopped()) {
		transform_.TransferMatrix(Matrix4x4::Identity());
		return;
	}

	playAnim_.AnimationPlayUpdate(skeleton_);

	skeleton_.SkeletonUpdate(transform_);
	skinCluster_.SkinClusterUpdate(skeleton_);
	transform_.TransferMatrix(Matrix4x4::Identity());
}

void Animation::Draw()
{
	animationBase_->DrawBase();

	auto commandList = DirectXEngine::GetCommandList();
	commandList->SetGraphicsRootConstantBufferView(4, LightManager::GetInstance()->GetDirectionalLightResource()->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(5, LightManager::GetInstance()->GetPointLightResource()->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(6, LightManager::GetInstance()->GetSpotLightResource()->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(9, CameraManager::GetInstance()->GetCameraResource()->GetGPUVirtualAddress());

	if (model_) {
		D3D12_VERTEX_BUFFER_VIEW vbvs[2] = {
			model_->GetVertexBuffer(),
			skinCluster_.GetInfluenceBufferView()
		};
		commandList->IASetVertexBuffers(0, 2, vbvs);
	}
}

void Animation::Play(size_t idx, float fadeTime)
{
	playAnim_.Play(idx, fadeTime);
}

bool Animation::PlayByName(const std::string& clipName, float fadeTime)
{
	return playAnim_.PlayByName(clipName, fadeTime);
}

void Animation::SetModel(const std::string& filePath)
{
	model_ = ModelManager::GetInstance()->FindModel(filePath);
}

void Animation::SetTexture(const std::string& directoryPath, const std::string& filePath)
{
	model_->SetTexture(directoryPath, filePath);
}

const size_t Animation::GetJointSize()const 
{
	return skeleton_.GetJoints().size();
}

const std::vector<WellForGPU>& Animation::GetWellForGPU()const 
{
	return skinCluster_.GetWellForGPU();
}

void Animation::MakeMaterialData()
{
	materialData_.color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialData_.enableDraw = true;
	materialData_.enableLighting = true;
	materialData_.uvTransform = Matrix4x4::Identity();
	materialData_.shininess = 20.0f;
	materialData_.environmentCoefficient = 0;
}
