#pragma once

#include <map>
#include <vector>
#include <string>
#include <memory>
#include <optional>

#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "Quaternion.h"
#include "Transform.h"

#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include <wrl.h>
using Microsoft::WRL::ComPtr;


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

struct AnimationData {
	std::string name;			// アニメーションの名前
	float duration;				// アニメーション全体の尺
	std::map<std::string, NodeAnimation> nodeAnimations;
};

struct WellForGPU {
	Matrix4x4 skeletonSpaceMatrix;
	Matrix4x4 skeletonSpaceInverseTransposeMatrix;
};