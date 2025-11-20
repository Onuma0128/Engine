#include "Joint.h"

int32_t Joint::CreateJoint(const Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints)
{
	Joint joint;
	joint.name_ = node.name;
	joint.localMatrix_ = node.localMatrix;
	joint.skeletonSpaceMatrix_ = Matrix4x4::Identity();
	joint.transform_ = node.transform;
	joint.index_ = int32_t(joints.size());
	joint.parent_ = parent;
	joints.push_back(joint);
	for (const Node& child : node.children) {
		int32_t childIndex = CreateJoint(child, joint.index_, joints);
		joints[joint.index_].children_.push_back(childIndex);
	}

	return joint.index_;
}

void Joint::TransformUpdate(const NodeAnimation& track, float animationTime, float duration)
{
	transform_.translation = CalculateValue(track.translate.keyframes, animationTime, duration);
	transform_.rotation = CalculateValue(track.rotate.keyframes, animationTime, duration);
	transform_.scale = CalculateValue(track.scale.keyframes, animationTime, duration);
}

Vector3 Joint::CalculateValue(const std::vector<KeyFrameVector3>& keys, float time, float clipDuration)
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

Quaternion Joint::CalculateValue(const std::vector<KeyFrameQuaternion>& keys, float time, float clipDuration)
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