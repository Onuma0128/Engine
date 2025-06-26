#pragma once
#include <memory>
#include <vector>
#include <unordered_map>

#include "AnimationBase.h"
#include "WorldTransform.h"

#include "AnimationStruct.h"
#include "ModelStruct.h"

class Model;
class Line3d;

class Animation
{
public:

	void Initialize(const std::string& filename);
	void SetSceneRenderer();
	void RemoveRenderer();

	void Update();

	void Draw();

	std::vector<AnimationData> LoadAnimationFile(const std::string& directoryPath, const std::string& filename);

	// 再生するアニメーションを選択
	void Play(size_t idx, float fadeTime = 0.3f);
	bool PlayByName(const std::string& clipName, float fadeTime = 0.3f);

	/*==================== アクセッサ ====================*/

	void SetModel(const std::string& filePath);
	void SetTexture(const std::string& directoryPath, const std::string& filePath);
	void SetColor(const Vector4& color);

	WorldTransform& GetTransform() { return transform_; }
	Model* GetModel() { return model_; }
	RenderOptions& GetRenderOptions() { return renderOptions_; }
	Vector4& GetColor() { return color_; }
	Material* GetMaterial() { return materialData_; }

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

	Vector4 color_ = { 1.0f,1.0f,1.0f,1.0f };

private:

	std::unique_ptr<AnimationBase> animationBase_ = nullptr;
	Model* model_ = nullptr;

	std::vector<AnimationData> animationDatas_;
	std::unordered_map<std::string, size_t> nameToIx_;
	AnimationBlendState blend_;
	size_t currentAnim_ = 0;
	float animationTime_ = 0.0f;

	SkinCluster skinCluster_;
	Skeleton skeleton_;

	std::unique_ptr<Line3d> line_;

	/*==================== マテリアル ====================*/

	// バッファリソース
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	// バッファリソース内のデータを指すポインタ
	Material* materialData_ = nullptr;

	// 描画するか
	RenderOptions renderOptions_;

};