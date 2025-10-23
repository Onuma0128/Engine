#pragma once

#include <optional>

#include "ModelStruct.h"

#include "Matrix4x4.h"
#include "Transform.h"

/// <summary>
/// ボーンのジョイントを表すクラス
/// </summary>
class Joint
{
public:

	/// <summary>
	/// ジョイントを作成する
	/// </summary>
	/// <param name="node"></アニメーションのノード>
	/// <param name="parent"></アニメーションの親子関係>
	/// <param name="joints"></アニメーションのジョイントデータ>
	/// <returns></returns>
	static int32_t CreateJoint(
		const Node& node, 
		const std::optional<int32_t>& parent, 
		std::vector<Joint>& joints
	);

	/// <summary>
	/// ジョイントの変形を更新
	/// </summary>
	/// <param name="track"></param>
	/// <param name="animationTime"></param>
	/// <param name="duration"></param>
	void TransformUpdate(const NodeAnimation& track, float animationTime, float duration);

	/// <summary>
	/// アクセッサ
	/// </summary>

	// 書き込み専用
	Transform3D& GetTransform() { return transform_; }
	Matrix4x4& GetLocalMatrix() { return localMatrix_; }
	Matrix4x4& GetSkeletonSpaceMatrix() { return skeletonSpaceMatrix_; }

    // 読み取り専用
    const Transform3D& GetTransform() const { return transform_; }
    const Matrix4x4& GetLocalMatrix() const { return localMatrix_; }
    const Matrix4x4& GetSkeletonSpaceMatrix() const { return skeletonSpaceMatrix_; }
    const std::string& GetName() const { return name_; }
    const std::vector<int32_t>& GetChildren() const { return children_; }
    int32_t GetIndex() const { return index_; }
    const std::optional<int32_t>& GetParent() const { return parent_; }

private:

	/// <summary>
	/// キーフレームからのVector3補間値を計算する
	/// </summary>
	/// <param name="keys"></param>
	/// <param name="time"></param>
	/// <param name="clipDuration"></param>
	/// <returns></returns>
	Vector3 CalculateValue(const std::vector<KeyFrameVector3>& keys, float time, float clipDuration);
	
	/// <summary>
	/// キーフレームからのQuaternion補間値を計算する
	/// </summary>
	/// <param name="keys"></param>
	/// <param name="time"></param>
	/// <param name="clipDuration"></param>
	/// <returns></returns>
	Quaternion CalculateValue(const std::vector<KeyFrameQuaternion>& keys, float time, float clipDuration);

private:

	Transform3D transform_;
	Matrix4x4 localMatrix_;
	Matrix4x4 skeletonSpaceMatrix_;
	std::string name_;
	std::vector<int32_t> children_;
	int32_t index_;
	std::optional<int32_t> parent_;

};

