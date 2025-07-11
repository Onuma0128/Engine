#pragma once
#include <memory>
#include <vector>
#include <unordered_map>

#include "AnimationBase.h"
#include "WorldTransform.h"
#include "Line3d.h"

#include "AnimationStruct.h"
#include "ModelStruct.h"

class Model;

class Animation
{
public:

	void Initialize(const std::string& filename);
	void SetSceneRenderer();
	void RemoveRenderer();

	void Update();
	void TransformUpdate() { transform_.TransferMatrix(Matrix4x4::Identity()); }

	void Draw();

	std::vector<AnimationData> LoadAnimationFile(const std::string& directoryPath, const std::string& filename);

	// 再生するアニメーションを選択
	void Play(size_t idx, float fadeTime = 0.3f);
	bool PlayByName(const std::string& clipName, float fadeTime = 0.3f);
	void SetAnimationTime(float animationTime) { animationTime_ = animationTime; }

	/*==================== アクセッサ ====================*/

	void SetModel(const std::string& filePath);
	void SetTexture(const std::string& directoryPath, const std::string& filePath);
	void SetColor(const Vector4& color);

	WorldTransform& GetTransform() { return transform_; }
	bool& GetTimeStop() { return timeStop_; }
	bool& GetReversePlay() { return reversePlay_; }
	Model* GetModel() { return model_; }
	size_t GetJointSize() { return skeleton_.joints.size(); }
	std::vector<WellForGPU>& GetWellForGPU() { return skinCluster_.mappedPalettes; }

	Vector4& GetColor() { return materialData_.color; }
	Material& GetMaterial() { return materialData_; }
	void SetDrawBone(bool flag);

private:

	Skeleton CreateSkeleton(const Node& rootNode);
	int32_t CreateJoint(const Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints);
	SkinCluster CreateSkinCluster(
		const ComPtr<ID3D12Device>& device, const Skeleton& skeleton, const ModelData& modelData
	);

	Vector3 CalculateValue(const std::vector<KeyFrameVector3>& keys, float time, float clipDuration);
	Quaternion CalculateValue(const std::vector<KeyFrameQuaternion>& keys, float time, float clipDuration);

	void SkeletonUpdate(Skeleton& skeleton);
	void SkinClusterUpdate(SkinCluster& skinCluster, const Skeleton& skeleton);
	void ApplyAnimation(Skeleton& skeleton, const AnimationData& animation, float animationTime, float duration);

	void MakeMaterialData();

	void LineUpdate();

protected:

	WorldTransform transform_;

private:

	std::unique_ptr<AnimationBase> animationBase_ = nullptr;
	Model* model_ = nullptr;

	std::vector<AnimationData> animationDatas_;
	std::unordered_map<std::string, size_t> nameToIx_;
	AnimationBlendState blend_;
	size_t currentAnim_ = 0;
	float animationTime_ = 0.0f;
	bool timeStop_ = false;			// アニメーションを止める
	bool stopped_ = false;			// アニメーションが止まっている
	bool reversePlay_ = false;		// アニメーションを逆再生させるか

	SkinCluster skinCluster_;
	Skeleton skeleton_;

	std::unique_ptr<Line3d> line_;

	/*==================== マテリアル ====================*/

	Material materialData_;

};