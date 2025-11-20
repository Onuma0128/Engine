#pragma once

#include <memory>

#include "JsonFunction.h"
#include "ParticleEmitter.h"

/// <summary>
/// パーティクルエディタクラス
/// </summary>
class ParticleEditor
{
public:

	// 初期化
	void Initialize(std::string filePath);

	// 更新
	void Update();

	// エディター側で作成したエミッターを取得
	ParticleEmitter::Emitter GetBaseEmitter()const { return baseEmitter_; }

	// テクスチャ・モデル・ブレンドモードの取得
	std::string GetTexture()const { return baseEmitter_.texture; }
	int GetModel()const { return static_cast<int>(baseEmitter_.model_); }
	int GetBlendMode()const { return static_cast<int>(baseEmitter_.blendMode_); }
	bool GetBillboard()const { return baseEmitter_.isBillboard; }

private:

	// 保存
	void Save();

private:

	JsonFunction parameters_;

	ParticleEmitter::Emitter baseEmitter_;

	// 選択中インデックス
	int textureIndex_ = 0;
	int modelIndex_ = 0;
	int blendIndex_ = 0;

};

