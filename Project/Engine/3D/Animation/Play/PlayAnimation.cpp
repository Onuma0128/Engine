#include "PlayAnimation.h"

#include "assimp//Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "DeltaTimer.h"
#include "animation/skeleton/Skeleton.h"

void PlayAnimation::Init(const ModelData& modelData)
{
	animationDatas_ = modelData.animations;
	for (size_t i = 0; i < animationDatas_.size(); ++i) {
		nameToIx_[animationDatas_[i].name] = i;
	}
	flags_.currentAnim = 0;
}

void PlayAnimation::AnimationPlayUpdate(Skeleton& skeleton)
{
	requestCooldown_ = std::max(0.0f, requestCooldown_ - DeltaTimer::GetDeltaTime());

	if (requestCooldown_ <= 0.0f && pendingIx_.has_value()) {
		// ここで反映（fadeTime は固定でもいいし保持してもOK）
		Play(*pendingIx_, 0.15f);
		pendingIx_.reset();
		requestCooldown_ = minRequestInterval_;
	}

	if (!blend_.active) {

		const AnimationData& clip = animationDatas_[flags_.currentAnim];
		if (flags_.reversePlay) {
			flags_.animationTime -= DeltaTimer::GetDeltaTime();
			if (flags_.animationTime < 0.0f) {
				if (flags_.timeStop) {
					flags_.animationTime = 0.0f;
					flags_.stopped = true;
				} else {
					flags_.animationTime = std::fmod(flags_.animationTime + clip.duration, clip.duration);
				}
			}
		} else {
			flags_.animationTime += DeltaTimer::GetDeltaTime();
			if (flags_.timeStop) {
				if (flags_.animationTime >= clip.duration) {
					flags_.animationTime = clip.duration;
					flags_.stopped = true;
				}
			} else {
				flags_.animationTime = std::fmod(flags_.animationTime, clip.duration);
			}
		}
		skeleton.ApplyAnimation(clip, flags_.animationTime, clip.duration);

	} else {

		const AnimationData& clipA = animationDatas_[blend_.fromIndex];
		blend_.fromTime += DeltaTimer::GetDeltaTime();
		blend_.fromTime = std::fmod(blend_.fromTime, clipA.duration);
		Skeleton poseA;		// スケルトンのコピー
		poseA.SetJoints(skeleton.GetJoints());
		poseA.ApplyAnimation(clipA, blend_.fromTime, clipA.duration);

		const AnimationData& clipB = animationDatas_[blend_.toIndex];
		blend_.toTime += DeltaTimer::GetDeltaTime();
		blend_.toTime = std::fmod(blend_.toTime, clipB.duration);
		Skeleton poseB;		// スケルトンのコピー
		poseB.SetJoints(skeleton.GetJoints());
		poseB.ApplyAnimation(clipB, blend_.toTime, clipB.duration);

		// 補完をする
		float alpha = blend_.time / blend_.duration;
		alpha = std::clamp(alpha, 0.0f, 1.0f);

		for (size_t j = 0; j < skeleton.GetJoints().size(); ++j) {
			skeleton.LerpTransformUpdate(j, alpha, poseA, poseB);
		}

		blend_.time += DeltaTimer::GetDeltaTime();
		if (blend_.time >= blend_.duration) {
			/*====== フェード完了 ======*/
			blend_.active = false;
			flags_.currentAnim = blend_.toIndex;
			flags_.animationTime = blend_.toTime;   // 継続時間を引き継ぐ
		}
	}
}

void PlayAnimation::Reset()
{
	animationDatas_.clear();
	nameToIx_.clear();
}

void PlayAnimation::ClearPending()
{
	pendingIx_.reset();      // 予約を消す
	requestCooldown_ = 0.0f; // クールダウンもリセットする
}

void PlayAnimation::Play(size_t idx, float fadeTime)
{
	if (idx >= animationDatas_.size()) { return; }

	// すでにそのアニメをターゲットにしてフェード中なら何もしない
	if (!blend_.active && idx == flags_.currentAnim) {
		return;
	}
	if (blend_.active && idx == blend_.toIndex) {
		return;
	}

	AnimationBlendState newBlend;

	if (!blend_.active) {
		// 通常時：現在のアニメ → 新しいアニメ へのフェード
		newBlend.fromIndex = flags_.currentAnim;
		newBlend.fromTime = flags_.animationTime;
	} else {
		// フェード中：今向かっているアニメ → 新しいアニメ へのフェードに上書き
		newBlend.fromIndex = blend_.toIndex;
		newBlend.fromTime = blend_.toTime;
	}

	newBlend.toIndex = idx;
	newBlend.toTime = 0.0f; // 新クリップは頭から
	newBlend.duration = std::fmax(0.001f, fadeTime);
	newBlend.time = 0.0f;
	newBlend.active = true;

	blend_ = newBlend;

	flags_.timeStop = false;
	flags_.stopped = false;
}

bool PlayAnimation::PlayByName(const std::string& clipName, float fadeTime)
{
	auto it = nameToIx_.find(clipName);
	if (it == nameToIx_.end()) { return false; }

	// クールダウン中は捨てずに予約
	if (requestCooldown_ > 0.0f) {
		pendingIx_ = it->second;
		return true;
	}

	Play(it->second, fadeTime);
	requestCooldown_ = minRequestInterval_;
	return true;
}

void PlayAnimation::ForcePlay(size_t idx, float startTime, bool keepPhase)
{
	if (idx >= animationDatas_.size()) { return; }

	ClearPending();

	// ブレンドを即時キャンセル
	blend_.active = false;
	blend_.time = 0.0f;

	// 新しい再生時間を決める
	float newTime = startTime;
	if (keepPhase) {
		const float oldDur = animationDatas_[flags_.currentAnim].duration;
		const float newDur = animationDatas_[idx].duration;
		float norm = (oldDur > 0.0f) ? (flags_.animationTime / oldDur) : 0.0f; // 0~∞ 想定
		norm = std::clamp(norm, 0.0f, 1.0f);
		newTime = norm * newDur;
	}
	// クランプ（wrapしない＝ハードカットの意図を維持）
	const float newDur = animationDatas_[idx].duration;
	newTime = std::clamp(newTime, 0.0f, newDur);

	// 状態を上書き
	flags_.currentAnim = idx;
	flags_.animationTime = newTime;
	flags_.timeStop = false;
	flags_.stopped = false;
}

bool PlayAnimation::ForcePlayByName(const std::string& clipName, float startTime, bool keepPhase)
{
	auto it = nameToIx_.find(clipName);
	if (it == nameToIx_.end()) { return false; }
	ForcePlay(it->second, startTime, keepPhase);
	return true;
}
