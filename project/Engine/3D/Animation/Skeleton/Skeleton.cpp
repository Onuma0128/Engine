#include "Skeleton.h"

#include <execution>
#include <numeric>

void Skeleton::CreateSkeleton(const WorldTransform& transform, const Node& rootNode)
{
	animTracksCache_.clear();

	item_.root = Joint::CreateJoint(rootNode, {}, item_.joints);

	for (const Joint& joint : item_.joints) {
		item_.jointMap.emplace(joint.GetName(), joint.GetIndex());
	}

	// DebugBoneの作成

#ifdef ENABLE_EDITOR

	debugBone_ = std::make_unique<DebugBone>();
	debugBone_->Init(debugBone_->GetPositions(transform, item_.joints));

#endif // ENABLE_EDITOR
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
#ifdef ENABLE_EDITOR

	debugBone_->SetPositions(debugBone_->GetPositions(transform, item_.joints));
	debugBone_->Update();

#endif // ENABLE_EDITOR
}

void Skeleton::ApplyAnimation(const AnimationData& animation, float animationTime, float duration)
{
	// index配列を用意
	std::vector<size_t> indices(item_.joints.size());
	// 0からsize分までの連続した値を作成する
	std::iota(indices.begin(), indices.end(), 0);
	const auto& tracks = GetOrBuildTrackCacke(animation);

	auto eval = [&](size_t jointIdx)
		{
			auto& joint = item_.joints[jointIdx];
			// 対応する NodeAnimation が無ければスキップ
			if (const NodeAnimation* track = tracks[jointIdx]) {
				joint.TransformUpdate(*track, animationTime, duration);
			}
		};

	if (indices.size() < 64) {
		for (size_t i : indices)eval(i);
	} else {
		std::for_each(std::execution::par_unseq, indices.begin(), indices.end(), eval);
	}
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
	animTracksCache_.clear();
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

const std::vector<const NodeAnimation*>& Skeleton::GetOrBuildTrackCacke(const AnimationData& animation)
{
	// クリップごとのキャッシュキーを生成
	size_t key = reinterpret_cast<size_t>(&animation);

	// 既存キャッシュを確認
	auto it = animTracksCache_.find(key);
	if (it != animTracksCache_.end()) {
		return it->second;
	}
	
	// ジョイント配列順のトラック表を作成
	std::vector<const NodeAnimation*> table(item_.joints.size(), nullptr);

	// 1回だけは名前を参照してトラックを探す
	for (size_t j = 0; j < item_.joints.size(); ++j) {
		const std::string& name = item_.joints[j].GetName();
		auto f = animation.nodeAnimations.find(name);
		if (f != animation.nodeAnimations.end()) {
			table[j] = &f->second;
		}
	}
	
	auto [inserted, _] = animTracksCache_.emplace(key, std::move(table));
	return inserted->second;
}
