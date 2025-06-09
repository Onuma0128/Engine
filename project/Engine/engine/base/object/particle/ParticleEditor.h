#pragma once

#include "JsonFunction.h"

#include "ParticleEmitter.h"

class ParticleEditor
{
public:
	
	void Initialize(std::string filePath);

	void Update(ParticleEmitter::Emitter emitter);

private:

	JsonFunction parameters_;

};

