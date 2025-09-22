#pragma once

#include <map>
#include <string>
#include <vector>
#include <memory>

#include "animation/joint/Joint.h"
#include "animation/debug/DebugBone.h"
#include "animation/play/PlayAnimation.h"

#include "WorldTransform.h"
#include "ModelStruct.h"
#include "AnimationStruct.h"

class Skeleton
{
private:

	struct SkeletonItem
	{
		int32_t root;
		std::map<std::string, int32_t> jointMap;
		std::vector<Joint> joints;
	};

public:

	// 初期化
	void CreateSkeleton(const WorldTransform& transform, const Node& rootNode);
	// 更新
	void SkeletonUpdate(const WorldTransform& transform);
	void DebugBoneUpdate(const WorldTransform& transform);
	// Joint更新
	void ApplyAnimation(const AnimationData& animation, float animationTime, float duration);
	// Jointの補完更新
	void LerpTransformUpdate(
		const size_t index,
		const float alpha,
		const Skeleton& poseA, 
		const Skeleton& poseB
	);
	void Reset();

	// SkeletonItem
	const SkeletonItem& GetItem()const { return item_; }
	// JointMapにJoint名があるかチェックする
	bool CheckJointName(const std::string name);
	// 各Joint取得用、読み込み用
	Joint& GetJoint(const size_t index) { return item_.joints[index]; }
	const std::vector<Joint>& GetJoints()const { return item_.joints; }
	void SetJoints(const std::vector<Joint>& joints) { item_.joints = joints; }
	void SetBoneEnableDraw(bool flag) { debugBone_->SetEnableDraw(flag); }

private:

	SkeletonItem item_;

	std::unique_ptr<DebugBone> debugBone_ = nullptr;

};

