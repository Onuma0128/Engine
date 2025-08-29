#pragma once

#include <map>
#include <string>
#include <vector>
#include <unordered_map>

#include "ModelStruct.h"
#include "AnimationStruct.h"

class Skeleton;

class PlayAnimation
{
private:

	struct AnimationBlendState {
		bool   active = false;		// フェード中？
		size_t fromIndex = 0;       // 直前のクリップ
		size_t toIndex = 0;			// 目標クリップ
		float  time = 0.0f;			// 経過
		float  duration = 0.3f;	    // フェード時間
		float  fromTime = 0.0f;		// fromClip の再生秒
		float  toTime = 0.0f;		// toClip   の再生秒
	};

	struct PlyaAnimationFlags {
		size_t currentAnim = 0;		// 今再生しているアニメーション
		float animationTime = 0.0f;	// 今再生位置を保存する
		bool timeStop = false;		// アニメーションを止める
		bool stopped = false;		// アニメーションが止まっている
		bool reversePlay = false;	// アニメーションを逆再生させるか
	};

public:

	void Init(const ModelData& modelData);

	void AnimationPlayUpdate(Skeleton& skeleton);

	void Reset();

	// 再生するアニメーションを選択
	void Play(size_t idx, float fadeTime);
	bool PlayByName(const std::string& clipName, float fadeTime);
	// 再生するアニメーションを強制変更
	void ForcePlay(size_t idx, float startTime = 0.0f, bool keepPhase = false);
	bool ForcePlayByName(const std::string& clipName, float startTime = 0.0f, bool keepPhase = false);
	void SetAnimationTime(float animationTime) { flags_.animationTime = animationTime; }

	bool& GetTimeStop() { return flags_.timeStop; }
	bool& GetStopped() { return flags_.stopped; }
	bool& GetReversePlay() { return flags_.reversePlay; }

private:

	std::vector<AnimationData> animationDatas_;
	std::unordered_map<std::string, size_t> nameToIx_;

	AnimationBlendState blend_;

	PlyaAnimationFlags flags_;

};

