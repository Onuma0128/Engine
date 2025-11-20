#pragma once

#include <map>
#include <string>
#include <vector>
#include <memory>

#include "animation/joint/Joint.h"
#include "animation/debugBone/DebugBone.h"
#include "animation/play/PlayAnimation.h"

#include "WorldTransform.h"
#include "ModelStruct.h"
#include "AnimationStruct.h"

/// <summary>
/// スケルトン
/// </summary>
class Skeleton
{
private:

	/// <summary>
	/// スケルトン項目
	/// </summary>
	struct SkeletonItem
	{
		int32_t root;
		std::map<std::string, int32_t> jointMap;
		std::vector<Joint> joints;
	};

public:

	/// <summary>
	/// スケルトンを作成する
	/// </summary>
	/// <param name="transform"></Transformを取得>
	/// <param name="rootNode"></アニメーションノード>
	void CreateSkeleton(const WorldTransform& transform, const Node& rootNode);

	/// <summary>
	/// スケルトンの更新
	/// </summary>
	/// <param name="transform"></param>
	void SkeletonUpdate(const WorldTransform& transform);

	/// <summary>
	/// デバッグ用ボーンの更新
	/// </summary>
	/// <param name="transform"></param>
	void DebugBoneUpdate(const WorldTransform& transform);

	/// <summary>
	/// アニメーションを適用、Jointの更新
	/// </summary>
	/// <param name="animation"></今再生しているアニメーションデータ>
	/// <param name="animationTime"></今再生しているアニメーション時間>
	/// <param name="duration"></param>
	void ApplyAnimation(const AnimationData& animation, float animationTime, float duration);

	/// <summary>
	/// Jointの補完更新
	/// </summary>
	/// <param name="index"></次再生するアニメーションIndex>
	/// <param name="alpha"></補完係数>
	/// <param name="poseA"></今再生中のスケルトン>
	/// <param name="poseB"></次再生するスケルトン>
	void LerpTransformUpdate(
		const size_t index,
		const float alpha,
		const Skeleton& poseA, 
		const Skeleton& poseB
	);
	/// <summary>
	/// スケルトンのリセット
	/// </summary>
	void Reset();

	/// <summary>
	/// アクセッサ
	/// </summary>

	// スケルトンの項目
	const SkeletonItem& GetItem()const { return item_; }
	// JointMapにJoint名があるかチェックする
	bool CheckJointName(const std::string name);
	// 各Joint取得用、読み込み用
	Joint& GetJoint(const size_t index) { return item_.joints[index]; }
	// 各Joint取得用、読み取り用
	const std::vector<Joint>& GetJoints()const { return item_.joints; }
	// 各Joint設定用
	void SetJoints(const std::vector<Joint>& joints) { item_.joints = joints; }
	// デバッグ用ボーンの描画有効/無効設定
	void SetBoneEnableDraw(bool flag) { debugBone_->SetEnableDraw(flag); }

	const std::vector<const NodeAnimation*>& GetOrBuildTrackCacke(const AnimationData& animationData);

private:

	SkeletonItem item_;

	std::unique_ptr<DebugBone> debugBone_ = nullptr;

	std::unordered_map<size_t, std::vector<const NodeAnimation*>> animTracksCache_;

};

