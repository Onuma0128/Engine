#include "DebugBone.h"

#include "animation/joint/Joint.h"

void DebugBone::Init(const std::vector<NumaEngine::Vector3>& positions)
{
    bone_ = std::make_unique<NumaEngine::Line3d>();
	bone_->Initialize(positions);
	bone_->GetMaterial().enableDraw = false;
}

void DebugBone::Update()
{
	bone_->Update();
}

std::vector<NumaEngine::Vector3> DebugBone::GetPositions(const WorldTransform& transform, const std::vector<Joint>& joints)
{
	std::vector<NumaEngine::Vector3> linePositions{};
	for (const Joint& joint : joints) {
		if (joint.GetParent()) {
            // 初期の位置を取得
			NumaEngine::Matrix4x4 parentMatrix = joints[*joint.GetParent()].GetSkeletonSpaceMatrix() * transform.matWorld_;
			NumaEngine::Matrix4x4 jointMatrix = joint.GetSkeletonSpaceMatrix() * transform.matWorld_;

			NumaEngine::Vector3 parentPos = NumaEngine::Vector3{}.Transform(parentMatrix);
			NumaEngine::Vector3 jointPos = NumaEngine::Vector3{}.Transform(jointMatrix);

			linePositions.push_back(parentPos);
			linePositions.push_back(jointPos);
		}
	}
	return linePositions;
}

