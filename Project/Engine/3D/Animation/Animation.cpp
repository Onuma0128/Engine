#include "Animation.h"

#include <execution>
#include <numeric>

#include "DirectXEngine.h"
#include "ModelInstanceRenderer.h"

#include "Object3d.h"
#include "Model.h"
#include "ModelManager.h"
#include "LightManager.h"
#include "CameraManager.h"

NumaEngine::Animation::~Animation()
{
	skinCluster_.Reset();
	skeleton_.Reset(); 
	playAnim_.Reset();
}

void NumaEngine::Animation::Initialize(const std::string& filename)
{
	skinCluster_.Reset();
	skeleton_.Reset();
	playAnim_.Reset();

	animationBase_ = std::make_unique<AnimationBase>();
	animationBase_->Initialize();

	transform_ = WorldTransform();
	SetModel(filename);
	MakeMaterialData();

	playAnim_.Init(model_->GetModelData());

	skeleton_.CreateSkeleton(transform_, model_->GetModelData().rootNode);

	skinCluster_.CreateSkinCluster(skeleton_, model_->GetModelData());
}

void NumaEngine::Animation::SetSceneRenderer()
{
   NumaEngine::ModelInstanceRenderer::GetInstance()->Push(this);
}

void NumaEngine::Animation::RemoveRenderer()
{
 NumaEngine::ModelInstanceRenderer::GetInstance()->Remove(this);
}

void NumaEngine::Animation::Update()
{
	if (playAnim_.GetStopped()) {
		transform_.TransferMatrix(Matrix4x4::Identity());
		return;
	}

	playAnim_.AnimationPlayUpdate(skeleton_);

	skeleton_.SkeletonUpdate(transform_);
	skinCluster_.SkinClusterUpdate(skeleton_);

	transform_.TransferMatrix(Matrix4x4::Identity());
	skeleton_.DebugBoneUpdate(transform_);
}

void NumaEngine::Animation::SetVertexBuffer()
{
	auto commandList = DirectXEngine::GetCommandList();

	if (model_) {
		D3D12_VERTEX_BUFFER_VIEW vbvs[2] = {
			model_->GetVertexBuffer(),
			skinCluster_.GetInfluenceBufferView()
		};
		commandList->IASetVertexBuffers(0, 2, vbvs);
	}
}

void NumaEngine::Animation::Draw()
{
	animationBase_->DrawBase();

	auto commandList = DirectXEngine::GetCommandList();
	commandList->SetGraphicsRootConstantBufferView(4, LightManager::GetInstance()->GetDirectionalLightResource()->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(5, LightManager::GetInstance()->GetPointLightResource()->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(6, LightManager::GetInstance()->GetSpotLightResource()->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(9, CameraManager::GetInstance()->GetCameraResource()->GetGPUVirtualAddress());

	SetVertexBuffer();
}

void NumaEngine::Animation::Play(size_t idx, float fadeTime)
{
	playAnim_.Play(idx, fadeTime);
}

bool NumaEngine::Animation::PlayByName(const std::string& clipName, float fadeTime)
{
	return playAnim_.PlayByName(clipName, fadeTime);
}

void NumaEngine::Animation::ForcePlay(size_t idx, float startTime, bool keepPhase) {
	playAnim_.ForcePlay(idx, startTime, keepPhase);
}

bool NumaEngine::Animation::ForcePlayByName(const std::string& clipName, float startTime, bool keepPhase) {
	return playAnim_.ForcePlayByName(clipName, startTime, keepPhase);
}

void NumaEngine::Animation::SetModel(const std::string& filePath)
{
	model_ = ModelManager::GetInstance()->FindModel(filePath);
}

void NumaEngine::Animation::SetTexture(const std::string& directoryPath, const std::string& filePath)
{
	model_->SetTexture(directoryPath, filePath);
}

const size_t NumaEngine::Animation::GetJointSize()const 
{
	return skeleton_.GetJoints().size();
}

const std::vector<WellForGPU>& NumaEngine::Animation::GetWellForGPU()const 
{
	return skinCluster_.GetWellForGPU();
}

void NumaEngine::Animation::MakeMaterialData()
{
	materialData_.color = NumaEngine::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialData_.enableDraw = true;
	materialData_.enableLighting = true;
	materialData_.outlineMask = false;
	materialData_.outlineSceneColor = false;
	materialData_.uvTransform = Matrix4x4::Identity();
	materialData_.shininess = 20.0f;
	materialData_.environmentCoefficient = 0;
	materialData_.shadowMap = true;
}

