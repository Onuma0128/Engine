#pragma once
#include <map>
#include <vector>
#include <string>
#include <memory>

#include "Model.h"

#include "Vector3.h"
#include "Quaternion.h"

class Object3d;

class Animation
{
public:

	template <typename tValue>
	struct KeyFrame {
		tValue value;		// キーフレームの値
		float time;			// キーフレームの時刻
	};
	using KeyFrameVector3 = KeyFrame<Vector3>;
	using KeyFrameQuaternion = KeyFrame<Quaternion>;


	template <typename tValue>
	struct AnimationCurve {
		std::vector<KeyFrame<tValue>> keyframes;
	};
	struct NodeAnimation {
		AnimationCurve<Vector3> scale;
		AnimationCurve<Quaternion> rotate;
		AnimationCurve<Vector3> translate;
	};

	struct AnimationData {
		float duration;	// アニメーション全体の尺
		std::map<std::string, NodeAnimation> nodeAnimations;
	};


	void Init();

	void Update();

	void Draw();

	AnimationData LoadAnimationFile(const std::string& directoryPath, const std::string& filename);

private:

	Vector3 CalculateValue(const std::vector<KeyFrameVector3>& keyframes, float time);

	Quaternion CalculateValue(const std::vector<KeyFrameQuaternion>& keyframes, float time);

private:

	std::unique_ptr<Object3d> model_ = nullptr;

	AnimationData animationData_;
	float animationTime_ = 0.0f;

};