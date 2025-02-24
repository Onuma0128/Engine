#include "Animation.h"

#include "assimp//Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "DirectXEngine.h"

#include "Object3d.h"
#include "Object3dBase.h"
#include "ModelManager.h"
#include "LightManager.h"
#include "CameraManager.h"
#include "Line3d.h"

#include "CreateBufferResource.h"
#include "imgui.h"

void Animation::Init(const std::string& directoryPath, const std::string& filename)
{
	ModelManager::GetInstance()->LoadModel(directoryPath, filename);
	this->object3dBase_ = Object3dBase::GetInstance();
	transform_ = WorldTransform();
	SetModel(filename);
	MakeMaterialData();

	animationData_ = LoadAnimationFile(directoryPath, filename);

	skeleton_ = CreateSkeleton(model_->GetModelData().rootNode);

	for (const Joint& joint : skeleton_.joints) {
		if (joint.parent) {
			// 初期の位置を取得
			Matrix4x4 parentMatrix = transform_.matWorld_ * skeleton_.joints[*joint.parent].skeletonSpaceMatrix;
			Matrix4x4 jointMatrix = transform_.matWorld_ * joint.skeletonSpaceMatrix;

			Vector3 parentPos = Vector3{}.Transform(parentMatrix);
			Vector3 jointPos = Vector3{}.Transform(jointMatrix);

			// Line3dを初期化
			std::unique_ptr<Line3d> line = std::make_unique<Line3d>();
			line->Initialize(parentPos, jointPos);
			lines_.push_back(std::move(line));
		}
	}
}

void Animation::Update()
{
	animationTime_ += 1.0f / 60.0f;
	animationTime_ = std::fmod(animationTime_, animationData_.duration);
	
	ApplyAnimation(skeleton_, animationData_, animationTime_);
	SkeletonUpdate(skeleton_);

	transform_.TransferMatrix(Matrix4x4::Identity());

	int32_t count = 0;
	for (const Joint& joint : skeleton_.joints) {
		if (joint.parent) {
			// 初期の位置を取得
			Matrix4x4 parentMatrix = skeleton_.joints[*joint.parent].skeletonSpaceMatrix * transform_.matWorld_;
			Matrix4x4 jointMatrix = joint.skeletonSpaceMatrix * transform_.matWorld_;

			Vector3 parentPos = Vector3{}.Transform(parentMatrix);
			Vector3 jointPos = Vector3{}.Transform(jointMatrix);

			lines_[count]->SetPosition(parentPos, jointPos);
			lines_[count]->Update();
			++count;
		}
	}
}

void Animation::Draw()
{
	object3dBase_->DrawBase();

	auto commandList = object3dBase_->GetDxEngine()->GetCommandList();
	commandList->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(1, transform_.GetConstBuffer()->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(3, LightManager::GetInstance()->GetDirectionalLightResource()->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(4, LightManager::GetInstance()->GetPointLightResource()->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(5, LightManager::GetInstance()->GetSpotLightResource()->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(6, CameraManager::GetInstance()->GetCameraResource()->GetGPUVirtualAddress());

	if (model_) {
		model_->Draw();
	}

	for (auto& line : lines_) {
		line->Draw();
	}
}

Animation::AnimationData Animation::LoadAnimationFile(const std::string& directoryPath, const std::string& filename)
{
	AnimationData animationData;
	Assimp::Importer importer;
	std::string filePath = directoryPath + "/" + filename;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), 0);
	assert(scene->mNumAnimations != 0);
	aiAnimation* animationAssimp = scene->mAnimations[0];
	animationData.duration = float(animationAssimp->mDuration / animationAssimp->mTicksPerSecond);

	for (uint32_t channelIndex = 0; channelIndex < animationAssimp->mNumChannels; ++channelIndex) {
		aiNodeAnim* nodeAnimationAssimp = animationAssimp->mChannels[channelIndex];
		NodeAnimation& nodeAnimation = animationData.nodeAnimations[nodeAnimationAssimp->mNodeName.C_Str()];
		
		// 座標
		for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumPositionKeys; ++keyIndex) {
			aiVectorKey& keyAssimp = nodeAnimationAssimp->mPositionKeys[keyIndex];
			KeyFrameVector3 keyframe;
			keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);
			keyframe.value = { -keyAssimp.mValue.x,keyAssimp.mValue.y,keyAssimp.mValue.z };
			nodeAnimation.translate.keyframes.push_back(keyframe);
		}
		// 回転
		for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumRotationKeys; ++keyIndex) {
			aiQuatKey& keyAssimp = nodeAnimationAssimp->mRotationKeys[keyIndex];
			KeyFrameQuaternion keyframe;
			keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);
			keyframe.value = { keyAssimp.mValue.x, -keyAssimp.mValue.y, -keyAssimp.mValue.z, keyAssimp.mValue.w };
			nodeAnimation.rotate.keyframes.push_back(keyframe);
		}
		// スケール
		for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumScalingKeys; ++keyIndex) {
			aiVectorKey& keyAssimp = nodeAnimationAssimp->mScalingKeys[keyIndex];
			KeyFrameVector3 keyframe;
			keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);
			keyframe.value = { keyAssimp.mValue.x, keyAssimp.mValue.y, keyAssimp.mValue.z };
			nodeAnimation.scale.keyframes.push_back(keyframe);
		}
	}

	return animationData;
}

Animation::Skeleton Animation::CreateSkeleton(const Model::Node& rootNode)
{
	Skeleton skeleton;
	skeleton.root = CreateJoint(rootNode, {}, skeleton.joints);

	for (const Joint& joint : skeleton.joints) {
		skeleton.jointMap.emplace(joint.name, joint.index);
	}

	return skeleton;
}

int32_t Animation::CreateJoint(const Model::Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints)
{
	Joint joint;
	joint.name = node.name;
	joint.localMatrix = node.localMatrix;
	joint.skeletonSpaceMatrix = Matrix4x4::Identity();
	joint.transform = node.transform;
	joint.index = int32_t(joints.size());
	joint.parent = parent;
	joints.push_back(joint);
	for (const Model::Node& child : node.children) {
		int32_t childIndex = CreateJoint(child, joint.index, joints);
		joints[joint.index].children.push_back(childIndex);
	}

	return joint.index;
}

void Animation::SkeletonUpdate(Skeleton& skeleton)
{
	for (Joint& joint : skeleton.joints) {
		joint.localMatrix = Matrix4x4::Affine(joint.transform.scale, joint.transform.rotation, joint.transform.translation);
		if (joint.parent) {
			joint.skeletonSpaceMatrix = joint.localMatrix * skeleton.joints[*joint.parent].skeletonSpaceMatrix;
		} else {
			joint.skeletonSpaceMatrix = joint.localMatrix;
		}
	}
}

void Animation::ApplyAnimation(Skeleton& skeleton, const AnimationData& animation, float animationTime)
{
	for (Joint& joint : skeleton.joints) {
		if (auto it = animation.nodeAnimations.find(joint.name); it != animation.nodeAnimations.end()) {
			const NodeAnimation& rootNodeAnimation = (*it).second;
			joint.transform.translation = CalculateValue(rootNodeAnimation.translate.keyframes, animationTime);
			joint.transform.rotation = CalculateValue(rootNodeAnimation.rotate.keyframes, animationTime);
			joint.transform.scale = CalculateValue(rootNodeAnimation.scale.keyframes, animationTime);
		}
	}
}

void Animation::SetModel(const std::string& filePath)
{
	model_ = ModelManager::GetInstance()->FindModel(filePath);
}

void Animation::SetTexture(const std::string& directoryPath, const std::string& filePath)
{
	model_->SetTexture(directoryPath, filePath);
}

void Animation::SetColor(const Vector4& color)
{
	materialData_->color = color;
}

Vector3 Animation::CalculateValue(const std::vector<KeyFrameVector3>& keyframes, float time)
{
	assert(!keyframes.empty());
	if (keyframes.size() == 1 || time <= keyframes[0].time) {
		return keyframes[0].value;
	}
	for (size_t index = 0; index < keyframes.size() - 1; ++index) {
		size_t nextIndex = index + 1;

		if (keyframes[index].time <= time && time <= keyframes[nextIndex].time) {
			float t = (time = keyframes[index].time) / (keyframes[nextIndex].time - keyframes[index].time);
			return Vector3::Lerp(keyframes[index].value, keyframes[nextIndex].value, t);
		}
	}

	return (*keyframes.rbegin()).value;
}

Quaternion Animation::CalculateValue(const std::vector<KeyFrameQuaternion>& keyframes, float time)
{
	assert(!keyframes.empty());
	if (keyframes.size() == 1 || time <= keyframes[0].time) {
		return keyframes[0].value;
	}
	for (size_t index = 0; index < keyframes.size() - 1; ++index) {
		size_t nextIndex = index + 1;

		if (keyframes[index].time <= time && time <= keyframes[nextIndex].time) {
			float t = (time - keyframes[index].time) / (keyframes[nextIndex].time - keyframes[index].time);
			return Quaternion::Lerp(keyframes[index].value, keyframes[nextIndex].value, t);
		}
	}

	return (*keyframes.rbegin()).value;
}

void Animation::MakeMaterialData()
{
	// マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
	materialResource_ = CreateBufferResource(object3dBase_->GetDxEngine()->GetDevice(), sizeof(Material)).Get();
	// 書き込むためのアドレスを取得
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	// 今回は白を書き込んでいく
	materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialData_->enableLighting = true;
	materialData_->uvTransform = Matrix4x4::Identity();
	materialData_->shininess = 20.0f;
}
