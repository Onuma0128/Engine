#include "DebugBone.h"

#include "animation/joint/Joint.h"

void DebugBone::Init(std::vector<Vector3> positions)
{
	bone_ = std::make_unique<Line3d>();
	bone_->Initialize(positions);
	bone_->GetMaterial().enableDraw = false;
}

void DebugBone::Update()
{
	bone_->Update();
}

std::vector<Vector3> DebugBone::GetPositions(const WorldTransform& transform, const std::vector<Joint>& joints)
{
	std::vector<Vector3> linePositions{};
	for (const Joint& joint : joints) {
		if (joint.GetParent()) {
			// 初期の位置を取得
			Matrix4x4 parentMatrix = joints[*joint.GetParent()].GetSkeletonSpaceMatrix() * transform.matWorld_;
			Matrix4x4 jointMatrix = joint.GetSkeletonSpaceMatrix() * transform.matWorld_;

			Vector3 parentPos = Vector3{}.Transform(parentMatrix);
			Vector3 jointPos = Vector3{}.Transform(jointMatrix);

			linePositions.push_back(parentPos);
			linePositions.push_back(jointPos);
		}
	}
	return linePositions;
}
