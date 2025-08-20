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

void PlayAnimation::Play(size_t idx, float fadeTime)
{
	if (idx >= animationDatas_.size() || idx == flags_.currentAnim) { return; }

	blend_.active = true;
	blend_.fromIndex = flags_.currentAnim;
	blend_.toIndex = idx;
	blend_.duration = std::fmax(0.001f, fadeTime);
	blend_.time = 0.0f;
	blend_.fromTime = flags_.animationTime;	// 現在の再生位置を保持
	blend_.toTime = 0.0f;					// 新クリップは 0 秒から
	flags_.timeStop = false;
	flags_.stopped = false;
}

bool PlayAnimation::PlayByName(const std::string& clipName, float fadeTime)
{
	auto it = nameToIx_.find(clipName);
	if (it == nameToIx_.end()) { return false; }          // 名前なし
	if (blend_.active) { return false; }

	Play(it->second, fadeTime);                           // index 切替 (前回答参照)
	return true;
}