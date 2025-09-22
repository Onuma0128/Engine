#include "Skeleton.h"

#include <execution>
#include <numeric>

void Skeleton::CreateSkeleton(const WorldTransform& transform, const Node& rootNode)
{
	item_.root = Joint::CreateJoint(rootNode, {}, item_.joints);

	for (const Joint& joint : item_.joints) {
		item_.jointMap.emplace(joint.GetName(), joint.GetIndex());
	}

	// DebugBoneの作成

#ifdef _DEBUG

	debugBone_ = std::make_unique<DebugBone>();
	debugBone_->Init(debugBone_->GetPositions(transform, item_.joints));

#endif // _DEBUG
}

void Skeleton::SkeletonUpdate(const WorldTransform& transform)
{
	for (Joint& joint : item_.joints) {
		Transform3D transform = joint.GetTransform();
		joint.GetLocalMatrix() = Matrix4x4::Affine(transform.scale, transform.rotation, transform.translation);
		if (joint.GetParent()) {
			joint.GetSkeletonSpaceMatrix() = joint.GetLocalMatrix() * item_.joints[*joint.GetParent()].GetSkeletonSpaceMatrix();
		} else {
			joint.GetSkeletonSpaceMatrix() = joint.GetLocalMatrix();
		}
	}
}

void Skeleton::DebugBoneUpdate(const WorldTransform& transform)
{
	// DebugBoneの更新
#ifdef _DEBUG

	debugBone_->SetPositions(debugBone_->GetPositions(transform, item_.joints));
	debugBone_->Update();

#endif // _DEBUG
}

void Skeleton::ApplyAnimation(const AnimationData& animation, float animationTime, float duration)
{
	// index配列を用意
	std::vector<size_t> indices(item_.joints.size());
	// 0からsize分までの連続した値を作成する
	std::iota(indices.begin(), indices.end(), 0);
	const auto& nodeAnims = animation.nodeAnimations;

	// par_unseqで並列処理
	std::for_each(std::execution::par_unseq, indices.begin(), indices.end(), [&](size_t jointIdx)
		{
			auto& joint = item_.joints[jointIdx];

			// 対応する NodeAnimation が無ければスキップ
			auto it = nodeAnims.find(joint.GetName());
			if (it == nodeAnims.end()) return;

			const NodeAnimation& track = it->second;

			joint.TransformUpdate(track, animationTime, duration);
		});
}

void Skeleton::LerpTransformUpdate(const size_t index, const float alpha, const Skeleton& poseA, const Skeleton& poseB)
{
	auto& dst = item_.joints[index];
	const auto& a = poseA.GetItem().joints[index];
	const auto& b = poseB.GetItem().joints[index];

	dst.GetTransform().translation = Vector3::Lerp(a.GetTransform().translation, b.GetTransform().translation, alpha);
	dst.GetTransform().scale = Vector3::Lerp(a.GetTransform().scale, b.GetTransform().scale, alpha);
	dst.GetTransform().rotation = Quaternion::Slerp(a.GetTransform().rotation, b.GetTransform().rotation, alpha);
}

void Skeleton::Reset()
{
	item_.jointMap.clear();
	item_.joints.clear();
	item_.root = 0;
}

bool Skeleton::CheckJointName(const std::string name)
{
	auto checkName = item_.jointMap.find(name);
	if (checkName == item_.jointMap.end()) {
		return true;
	}
	return false;
}
