#pragma once
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include <wrl.h>

#include <map>
#include <vector>
#include <string>
#include <memory>
#include <optional>

#include "Model.h"

#include "WorldTransform.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "Quaternion.h"

using Microsoft::WRL::ComPtr;

class Object3dBase;
class Line3d;

class Animation
{
public:

	/// <summary>
	/// AnimationのTransform用構造体
	/// </summary>
	template <typename tValue>
	struct KeyFrame {
		tValue value;		// キーフレームの値
		float time;			// キーフレームの時刻
	};
	using KeyFrameVector3 = KeyFrame<Vector3>;
	using KeyFrameQuaternion = KeyFrame<Quaternion>;

	/// <summary>
	/// NodeのAnimation用構造体
	/// </summary>
	template <typename tValue>
	struct AnimationCurve {
		std::vector<KeyFrame<tValue>> keyframes;
	};
	struct NodeAnimation {
		AnimationCurve<Vector3> scale;
		AnimationCurve<Quaternion> rotate;
		AnimationCurve<Vector3> translate;
	};

	/// <summary>
	/// Animationを表現する用構造体
	/// </summary>
	struct AnimationData {
		float duration;	// アニメーション全体の尺
		std::map<std::string, NodeAnimation> nodeAnimations;
	};

	/// <summary>
	/// Material用構造体
	/// </summary>
	struct Material {
		Vector4 color;
		int32_t enableLighting;
		float padding[3];
		Matrix4x4 uvTransform;
		float shininess;
	};

	/// <summary>
	/// JointとSkeleton用構造体
	/// </summary>
	struct Joint {
		Transform3D transform;
		Matrix4x4 localMatrix;
		Matrix4x4 skeletonSpaceMatrix;
		std::string name;
		std::vector<int32_t> children;
		int32_t index;
		std::optional<int32_t> parent;
	};
	struct Skeleton {
		int32_t root;
		std::map<std::string, int32_t> jointMap;
		std::vector<Joint> joints;
	};


	void Init(const std::string& directoryPath, const std::string& filename);

	void Update();

	void Draw();

	AnimationData LoadAnimationFile(const std::string& directoryPath, const std::string& filename);

	/*==================== アクセッサ ====================*/

	void SetModel(const std::string& filePath);
	void SetTexture(const std::string& directoryPath, const std::string& filePath);
	void SetColor(const Vector4& color);

	WorldTransform& GetTransform() { return transform_; }

private:

	Skeleton CreateSkeleton(const Model::Node& rootNode);
	int32_t CreateJoint(const Model::Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints);

	Vector3 CalculateValue(const std::vector<KeyFrameVector3>& keyframes, float time);
	Quaternion CalculateValue(const std::vector<KeyFrameQuaternion>& keyframes, float time);

	void SkeletonUpdate(Skeleton& skeleton);
	void ApplyAnimation(Skeleton& skeleton, const AnimationData& animation, float animationTime);

	void MakeMaterialData();

protected:

	WorldTransform transform_;

private:

	Object3dBase* object3dBase_ = nullptr;
	Model* model_ = nullptr;

	AnimationData animationData_;
	float animationTime_ = 0.0f;

	Skeleton skeleton_;

	std::vector<std::unique_ptr<Line3d>> lines_;

	/*==================== マテリアル ====================*/

	// バッファリソース
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	// バッファリソース内のデータを指すポインタ
	Material* materialData_ = nullptr;

};