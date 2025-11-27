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

/// <summary>
/// アニメーションクラス
/// </summary>
class Animation
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Animation();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="filename"></param>
	void Initialize(const std::string& filename);

	/// <summary>
	/// シーンレンダラーを設定
	/// </summary>
	void SetSceneRenderer();
	/// <summary>
	/// シーンレンダラーを削除
	/// </summary>
	void RemoveRenderer();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	void TransformUpdate() { transform_.TransferMatrix(Matrix4x4::Identity()); }

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 再生するアニメーションを選択
	/// </summary>
	/// <param name="idx"></アニメーションさせるIndex>
	/// <param name="fadeTime"></今アニメから変えるアニメのチェンジフレーム>
	void Play(size_t idx, float fadeTime = 0.3f);
	bool PlayByName(const std::string& clipName, float fadeTime = 0.3f);

	/// <summary>
	/// 再生するアニメーションを強制変更
	/// </summary>
	/// <param name="idx"></アニメーションさせるIndex>
	/// <param name="startTime"></param>
	/// <param name="keepPhase"></param>
	void ForcePlay(size_t idx, float startTime = 0.0f, bool keepPhase = false);
	bool ForcePlayByName(const std::string& clipName, float startTime = 0.0f, bool keepPhase = false);

	/// <summary>
	/// アクセッサ
	/// </summary>

	// 頂点バッファ設定
	void SetVertexBuffer();
	// モデル設定
	void SetModel(const std::string& filePath);
	// テクスチャ設定
	void SetTexture(const std::string& directoryPath, const std::string& filePath);
	// ボーンのデバッグ描画有効/無効設定
	void SetBoneEnableDraw(bool flag) { skeleton_.SetBoneEnableDraw(flag); }
	// 再生中のアニメーションデータを取得
	void SetAnimationTime(float animationTime) { playAnim_.SetAnimationTime(animationTime); }

	// ワールド変換取得
	WorldTransform& GetTransform() { return transform_; }
	// 取得中のスケルトン
	bool& GetTimeStop() { return playAnim_.GetTimeStop(); }
	// アニメーションが止まっているか取得
	bool& GetReversePlay() { return playAnim_.GetReversePlay(); }
	// モデル取得
	Model* GetModel() { return model_; }
	// マテリアルデータ取得
	Material& GetMaterial() { return materialData_; }

	// ジョイント数取得
	const size_t GetJointSize()const;
	// スケルトン取得
	const std::vector<WellForGPU>& GetWellForGPU()const;

private:
	
	/// <summary>
	/// マテリアルデータを作成
	/// </summary>
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