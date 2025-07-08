#include "Animation.h"

#include "assimp//Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include <execution>
#include <numeric>

#include "DirectXEngine.h"
#include "SrvManager.h"
#include "TextureManager.h"
#include "DeltaTimer.h"

#include "Object3d.h"
#include "Model.h"
#include "ModelManager.h"
#include "LightManager.h"
#include "CameraManager.h"
#include "Line3d.h"

#include "CreateBufferResource.h"

void Animation::Initialize(const std::string& filename)
{
	animationBase_ = std::make_unique<AnimationBase>();
	animationBase_->Initialize();

	transform_ = WorldTransform();
	SetModel(filename);
	MakeMaterialData();

	animationDatas_ = model_->GetModelData().animations;
	for (size_t i = 0; i < animationDatas_.size(); ++i) {
		nameToIx_[animationDatas_[i].name] = i;
	}
	currentAnim_ = 0;

	skeleton_ = CreateSkeleton(model_->GetModelData().rootNode);
	skinCluster_ = CreateSkinCluster(DirectXEngine::GetDevice(), skeleton_, model_->GetModelData());

	std::vector<Vector3> linePositions{};
	for (const Joint& joint : skeleton_.joints) {
		if (joint.parent) {
			// 初期の位置を取得
			Matrix4x4 parentMatrix = transform_.matWorld_ * skeleton_.joints[*joint.parent].skeletonSpaceMatrix;
			Matrix4x4 jointMatrix = transform_.matWorld_ * joint.skeletonSpaceMatrix;

			Vector3 parentPos = Vector3{}.Transform(parentMatrix);
			Vector3 jointPos = Vector3{}.Transform(jointMatrix);

			linePositions.push_back(parentPos);
			linePositions.push_back(jointPos);
		}
	}

#ifdef _DEBUG
	// Line3dを初期化
	line_ = std::make_unique<Line3d>();
	line_->Initialize(linePositions);
#endif // _DEBUG
}

void Animation::SetSceneRenderer()
{
	DirectXEngine::GetModelRenderer()->Push(this);
}

void Animation::RemoveRenderer()
{
	DirectXEngine::GetModelRenderer()->Remove(this);
	if (line_ == nullptr) { return; }
	DirectXEngine::GetSceneRenderer()->SetRemoveList(line_.get());
}

void Animation::Update()
{
	if (stopped_) {
		transform_.TransferMatrix(Matrix4x4::Identity());
		if (line_ == nullptr) { return; }
		LineUpdate();
		return;
	}

	if (!blend_.active) {

		const AnimationData& clip = animationDatas_[currentAnim_];
		if (reversePlay_) {
			animationTime_ -= DeltaTimer::GetDeltaTime();
			if (animationTime_ < 0.0f) {
				if (timeStop_) {
					animationTime_ = 0.0f;
					stopped_ = true;
				} else {
					animationTime_ = std::fmod(animationTime_ + clip.duration, clip.duration);
				}
			}
		} else {
			animationTime_ += DeltaTimer::GetDeltaTime();
			if (timeStop_) {
				if (animationTime_ >= clip.duration) {
					animationTime_ = clip.duration;
					stopped_ = true;
				}
			} else {
				animationTime_ = std::fmod(animationTime_, clip.duration);
			}
		}

		ApplyAnimation(skeleton_, clip, animationTime_, clip.duration);

	} else {

		const AnimationData& clipA = animationDatas_[blend_.fromIndex];
		blend_.fromTime += DeltaTimer::GetDeltaTime();
		blend_.fromTime = std::fmod(blend_.fromTime, clipA.duration);
		Skeleton poseA = skeleton_;		// スケルトンのコピー
		ApplyAnimation(poseA, clipA, blend_.fromTime, clipA.duration);

		const AnimationData& clipB = animationDatas_[blend_.toIndex];
		blend_.toTime += DeltaTimer::GetDeltaTime();
		blend_.toTime = std::fmod(blend_.toTime, clipB.duration);
		Skeleton poseB = skeleton_;		// スケルトンのコピー
		ApplyAnimation(poseB, clipB, blend_.toTime, clipB.duration);

		// 補完をする
		float alpha = blend_.time / blend_.duration;
		alpha = std::clamp(alpha, 0.0f, 1.0f);

		for (size_t j = 0; j < skeleton_.joints.size(); ++j) {
			auto& dst = skeleton_.joints[j];
			const auto& a = poseA.joints[j];
			const auto& b = poseB.joints[j];

			dst.transform.translation = Vector3::Lerp(a.transform.translation, b.transform.translation, alpha);
			dst.transform.scale = Vector3::Lerp(a.transform.scale, b.transform.scale, alpha);
			dst.transform.rotation = Quaternion::Slerp(a.transform.rotation, b.transform.rotation, alpha);
		}

		blend_.time += DeltaTimer::GetDeltaTime();
		if (blend_.time >= blend_.duration) {
			/*====== フェード完了 ======*/
			blend_.active = false;
			currentAnim_ = blend_.toIndex;
			animationTime_ = blend_.toTime;   // 継続時間を引き継ぐ
		}
	}

	SkeletonUpdate(skeleton_);
	SkinClusterUpdate(skinCluster_, skeleton_);
	transform_.TransferMatrix(Matrix4x4::Identity());
	if (line_ == nullptr) { return; }
	LineUpdate();
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
			skinCluster_.influenceBufferView
		};
		commandList->IASetVertexBuffers(0, 2, vbvs);
	}
}

std::vector<AnimationData> Animation::LoadAnimationFile(const std::string& directoryPath, const std::string& filename)
{
	std::vector<AnimationData> animationDatas;
	Assimp::Importer importer;
	std::string filePath = directoryPath + "/" + filename;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), 0);
	assert(scene->mNumAnimations != 0);

	for (uint32_t animationIndex = 0; animationIndex < scene->mNumAnimations; ++animationIndex) {

		aiAnimation* animationAssimp = scene->mAnimations[animationIndex];
		AnimationData animationData;
		animationData.name = animationAssimp->mName.C_Str();
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
		animationDatas.push_back(std::move(animationData));
	}

	return animationDatas;
}

void Animation::Play(size_t idx, float fadeTime)
{
	if (idx >= animationDatas_.size() || idx == currentAnim_) { return; }

	blend_.active = true;
	blend_.fromIndex = currentAnim_;
	blend_.toIndex = idx;
	blend_.duration = std::fmax(0.001f, fadeTime);
	blend_.time = 0.0f;
	blend_.fromTime = animationTime_;   // 現在の再生位置を保持
	blend_.toTime = 0.0f;             // 新クリップは 0 秒から
	timeStop_ = false;
	stopped_ = false;
}

bool Animation::PlayByName(const std::string& clipName, float fadeTime)
{
	auto it = nameToIx_.find(clipName);
	if (it == nameToIx_.end()) { return false; }          // 名前なし
	if (blend_.active) { return false; }

	Play(it->second, fadeTime);                           // index 切替 (前回答参照)
	return true;
}

Skeleton Animation::CreateSkeleton(const Node& rootNode)
{
	Skeleton skeleton;
	skeleton.root = CreateJoint(rootNode, {}, skeleton.joints);

	for (const Joint& joint : skeleton.joints) {
		skeleton.jointMap.emplace(joint.name, joint.index);
	}

	return skeleton;
}

int32_t Animation::CreateJoint(const Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints)
{
	Joint joint;
	joint.name = node.name;
	joint.localMatrix = node.localMatrix;
	joint.skeletonSpaceMatrix = Matrix4x4::Identity();
	joint.transform = node.transform;
	joint.index = int32_t(joints.size());
	joint.parent = parent;
	joints.push_back(joint);
	for (const Node& child : node.children) {
		int32_t childIndex = CreateJoint(child, joint.index, joints);
		joints[joint.index].children.push_back(childIndex);
	}

	return joint.index;
}

SkinCluster Animation::CreateSkinCluster(const ComPtr<ID3D12Device>& device, const Skeleton& skeleton, const ModelData& modelData)
{
	SkinCluster skinCluster;
	SrvManager* srvManager = SrvManager::GetInstance();

	// **Influenceリソースの作成**
	size_t influenceSize = modelData.vertices.size() * sizeof(VertexInfluence);
	skinCluster.infuenceResource = CreateBufferResource(device, influenceSize);

	// **Influenceのマッピング**
	VertexInfluence* mappedInfluence = nullptr;
	skinCluster.infuenceResource->Map(0, nullptr, reinterpret_cast<void**>(&mappedInfluence));
	std::memset(mappedInfluence, 0, influenceSize);
	skinCluster.mappedInfluence = std::span<VertexInfluence>(mappedInfluence, modelData.vertices.size());

	// **Influence バッファビューの設定**
	skinCluster.influenceBufferView.BufferLocation = skinCluster.infuenceResource->GetGPUVirtualAddress();
	skinCluster.influenceBufferView.SizeInBytes = static_cast<UINT>(influenceSize);
	skinCluster.influenceBufferView.StrideInBytes = sizeof(VertexInfluence);

	skinCluster.mappedPalettes.resize(skeleton.joints.size());
	for (size_t i = 0; i < skinCluster.mappedPalettes.size(); ++i) {
		skinCluster.mappedPalettes[i].skeletonSpaceMatrix = Matrix4x4::Identity();
		skinCluster.mappedPalettes[i].skeletonSpaceInverseTransposeMatrix = Matrix4x4::Identity();
	}
	skinCluster.inverseBindPoseMatrices.resize(skeleton.joints.size());
	for (size_t i = 0; i < skinCluster.inverseBindPoseMatrices.size(); ++i) {
		skinCluster.inverseBindPoseMatrices[i] = Matrix4x4::Identity();
	}

	for (const auto& jointWeight : model_->GetModelData().skinClusterData) {
		// jointWeight.firstはjoint名なので、skeletonに対象となるjointが含まれているか判断
		auto it = skeleton.jointMap.find(jointWeight.first);
		if (it == skeleton.jointMap.end()) { // そんな名前のjointは存在しない。なので次に回す
			continue;
		}
		// (*it).secondにはjointのindexが入っているので、該当のindexのinverseBindPoseMatrixを代入
		skinCluster.inverseBindPoseMatrices[(*it).second] = jointWeight.second.inverseBindPosMatrix;
		for (const auto& vertexWeight : jointWeight.second.vertexWeights) {
			auto& currentInfluence = skinCluster.mappedInfluence[vertexWeight.vertexIndex];
			// 空いている場所に入れる
			for (uint32_t index = 0; index < kNumMaxInfluence; ++index) {
				// weight == 0.0fが開いている状態なので、その場所にweightとjointのindexを代入
				if (currentInfluence.weights[index] == 0.0f) {
					currentInfluence.weights[index] = vertexWeight.weight;
					currentInfluence.jointIndices[index] = (*it).second;
					break;
				}
			}
		}
	}

	return skinCluster;
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

void Animation::SkinClusterUpdate(SkinCluster& skinCluster, const Skeleton& skeleton)
{
	for (size_t jointIndex = 0; jointIndex < skeleton.joints.size(); ++jointIndex) {
		assert(jointIndex < skinCluster.inverseBindPoseMatrices.size());
		skinCluster.mappedPalettes[jointIndex].skeletonSpaceMatrix =
			skinCluster.inverseBindPoseMatrices[jointIndex] * skeleton.joints[jointIndex].skeletonSpaceMatrix;
		skinCluster.mappedPalettes[jointIndex].skeletonSpaceInverseTransposeMatrix =
			Matrix4x4::Inverse(skinCluster.mappedPalettes[jointIndex].skeletonSpaceMatrix).Transpose();
	}
}

void Animation::ApplyAnimation(Skeleton& skeleton, const AnimationData& animation, float animationTime, float duration)
{
	// index配列を用意
	std::vector<size_t> indices(skeleton_.joints.size());
	// 0からsize分までの連続した値を作成する
	std::iota(indices.begin(), indices.end(), 0);
	const auto& nodeAnims = animation.nodeAnimations;

	// par_unseqで並列処理
	std::for_each(std::execution::par_unseq,indices.begin(), indices.end(),[&](size_t jointIdx)
    {
        auto& joint = skeleton.joints[jointIdx];

        // 対応する NodeAnimation が無ければスキップ
        auto it = nodeAnims.find(joint.name);
        if (it == nodeAnims.end()) return;

        const NodeAnimation& track = it->second;

        joint.transform.translation =
            CalculateValue(track.translate.keyframes, animationTime, duration);
        joint.transform.rotation   =
            CalculateValue(track.rotate.keyframes, animationTime, duration);
        joint.transform.scale      =
            CalculateValue(track.scale.keyframes, animationTime, duration);
    });
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
	materialData_.color = color;
}

void Animation::SetDrawBone(bool flag)
{
	if (line_ == nullptr) { return; }
	line_->GetRenderOptions().enabled = false;
}

Vector3 Animation::CalculateValue(const std::vector<KeyFrameVector3>& keys, float time, float clipDuration)
{
	assert(!keys.empty());

	// 最後から最初の補間を処理
	if (time < keys.front().time) {
		const auto& last = keys.back();
		const auto& first = keys.front();

		float span = (clipDuration - last.time) + first.time;
		float t = (time + clipDuration - last.time) / span;
		return Vector3::Lerp(last.value, first.value, t);
	}

	// 既存ロジック
	if (keys.size() == 1 || time <= keys[0].time) { return keys[0].value; }
	for (size_t i = 0; i < keys.size() - 1; ++i) {
		size_t j = i + 1;
		if (keys[i].time <= time && time <= keys[j].time) {
			float t = (time - keys[i].time) / (keys[j].time - keys[i].time);
			return Vector3::Lerp(keys[i].value, keys[j].value, t);
		}
	}
	return keys.back().value;
}

Quaternion Animation::CalculateValue(const std::vector<KeyFrameQuaternion>& keys, float time, float clipDuration)
{
	assert(!keys.empty());

	// 最後から最初の補間を処理
	if (time < keys.front().time) {
		const auto& last = keys.back();
		const auto& first = keys.front();

		float span = (clipDuration - last.time) + first.time;
		float t = (time + clipDuration - last.time) / span;
		return Quaternion::Slerp(last.value, first.value, t);
	}

	// 既存ロジック
	if (keys.size() == 1 || time <= keys[0].time) { return keys[0].value; }
	for (size_t i = 0; i < keys.size() - 1; ++i) {
		size_t j = i + 1;
		if (keys[i].time <= time && time <= keys[j].time) {
			float t = (time - keys[i].time) / (keys[j].time - keys[i].time);
			return Quaternion::Slerp(keys[i].value, keys[j].value, t);
		}
	}
	return keys.back().value;
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

void Animation::LineUpdate()
{
	int32_t count = 0;
	std::vector<Vector3> linePositions{};
	for (const Joint& joint : skeleton_.joints) {
		if (joint.parent) {
			// 初期の位置を取得
			Matrix4x4 parentMatrix = skeleton_.joints[*joint.parent].skeletonSpaceMatrix * transform_.matWorld_;
			Matrix4x4 jointMatrix = joint.skeletonSpaceMatrix * transform_.matWorld_;

			Vector3 parentPos = Vector3{}.Transform(parentMatrix);
			Vector3 jointPos = Vector3{}.Transform(jointMatrix);

			linePositions.push_back(parentPos);
			linePositions.push_back(jointPos);

			++count;
		}
	}

	line_->SetPositions(linePositions);
	line_->Update();
}
