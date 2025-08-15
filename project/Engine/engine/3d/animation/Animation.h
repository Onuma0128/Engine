#pragma once
#include <memory>
#include <vector>

#include "AnimationBase.h"
#include "WorldTransform.h"

#include "AnimationStruct.h"
#include "ModelStruct.h"

#include "animation/skeleton/Skeleton.h"
#include "animation/skinCluster/SkinCluster.h"
#include "animation/play/PlayAnimation.h"

class Model;

class Animation
{
public:

	void Initialize(const std::string& filename);
	void SetSceneRenderer();
	void RemoveRenderer();

	void Update();
	void TransformUpdate() { transform_.TransferMatrix(Matrix4x4::Identity()); }

	void SetVertexBuffer();
	void Draw();

	// 再生するアニメーションを選択
	void Play(size_t idx, float fadeTime = 0.3f);
	bool PlayByName(const std::string& clipName, float fadeTime = 0.3f);
	void SetAnimationTime(float animationTime) { playAnim_.SetAnimationTime(animationTime); }

	/*==================== アクセッサ ====================*/

	void SetModel(const std::string& filePath);
	void SetTexture(const std::string& directoryPath, const std::string& filePath);
	void SetBoneEnableDraw(bool flag) { skeleton_.SetBoneEnableDraw(flag); }

	WorldTransform& GetTransform() { return transform_; }
	bool& GetTimeStop() { return playAnim_.GetTimeStop(); }
	bool& GetReversePlay() { return playAnim_.GetReversePlay(); }
	Model* GetModel() { return model_; }
	Material& GetMaterial() { return materialData_; }

	const size_t GetJointSize()const;
	const std::vector<WellForGPU>& GetWellForGPU()const;

private:
	
	void MakeMaterialData();

protected:

	WorldTransform transform_;

private:

	std::unique_ptr<AnimationBase> animationBase_ = nullptr;
	Model* model_ = nullptr;

	PlayAnimation playAnim_;

	SkinCluster skinCluster_;
	Skeleton skeleton_;

	/*==================== マテリアル ====================*/

	Material materialData_;

};