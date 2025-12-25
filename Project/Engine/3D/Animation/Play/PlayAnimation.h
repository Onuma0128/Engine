#pragma once

#define NOMINMAX

#include <map>
#include <string>
#include <vector>
#include <unordered_map>

#include "ModelStruct.h"
#include "AnimationStruct.h"

class Skeleton;

/// <summary>
/// アニメーションの再生を管理するクラス
/// </summary>
class PlayAnimation
{
private:

	/// <summary>
	/// アニメーションのブレンド状態を表す構造体
	/// </summary>
	struct AnimationBlendState {
		bool   active = false;		// フェード中？
		size_t fromIndex = 0;       // 直前のクリップ
		size_t toIndex = 0;			// 目標クリップ
		float  time = 0.0f;			// 経過
		float  duration = 0.3f;	    // フェード時間
		float  fromTime = 0.0f;		// fromClip の再生秒
		float  toTime = 0.0f;		// toClip   の再生秒
	};

	/// <summary>
	/// アニメーションのフラグを表す構造体
	/// </summary>
	struct PlayAnimationFlags {
		size_t currentAnim = 0;		// 今再生しているアニメーション
		float animationTime = 0.0f;	// 今再生位置を保存する
		bool timeStop = false;		// アニメーションを止める
		bool stopped = false;		// アニメーションが止まっている
		bool reversePlay = false;	// アニメーションを逆再生させるか
	};

public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="modelData"></param>
	void Init(const ModelData& modelData);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="skeleton"></param>
	void AnimationPlayUpdate(Skeleton& skeleton);

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	// 再生するアニメーションを選択

	/// <summary>
	/// アニメーションを再生する
	/// </summary>
	/// <param name="idx"></アニメーションさせるIndex>
	/// <param name="fadeTime"></今アニメから変えるアニメのチェンジフレーム>
	void Play(size_t idx, float fadeTime);
	bool PlayByName(const std::string& clipName, float fadeTime);

	/// <summary>
	/// アニメーションを強制的に再生する
	/// </summary>
	/// <param name="idx"></アニメーションさせるIndex>
	/// <param name="startTime"></param>
	/// <param name="keepPhase"></param>
	void ForcePlay(size_t idx, float startTime = 0.0f, bool keepPhase = false);
	bool ForcePlayByName(const std::string& clipName, float startTime = 0.0f, bool keepPhase = false);

	/// <summary>
	/// アクセッサ
	/// </summary>
	
	// 再生中のアニメーションデータを取得
	void SetAnimationTime(float animationTime) { flags_.animationTime = animationTime; }
	// 再生中のアニメーションデータを取得
	bool& GetTimeStop() { return flags_.timeStop; }
	// アニメーションが止まっているか取得
	bool& GetStopped() { return flags_.stopped; }
	// 逆再生フラグ取得
	bool& GetReversePlay() { return flags_.reversePlay; }

private:

	std::vector<AnimationData> animationDatas_;
	std::unordered_map<std::string, size_t> nameToIx_;

	AnimationBlendState blend_;

	std::optional<size_t> pendingIx_;
	float requestCooldown_ = 0.0f;
	float minRequestInterval_ = 0.08f;

	PlayAnimationFlags flags_;

};

