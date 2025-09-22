#pragma once

#include <memory>

#include "JsonFunction.h"
#include "ParticleEmitter.h"

class ParticleEditor
{
public:
	
	void Initialize(std::string filePath);

	void Update();

	ParticleEmitter::Emitter GetBaseEmitter()const { return baseEmitter_; }

	std::string GetTexture()const { return baseEmitter_.texture; }
	int GetModel()const { return static_cast<int>(baseEmitter_.model_); }
	int GetBlendMode()const { return static_cast<int>(baseEmitter_.blendMode_); }
	bool GetBillboard()const { return baseEmitter_.isBillboard; }

private:

	void Save();

private:

	JsonFunction parameters_;

	ParticleEmitter::Emitter baseEmitter_;

	// 選択中インデックス
	int textureIndex_ = 0;
	int modelIndex_ = 0;
	int blendIndex_ = 0;

};

