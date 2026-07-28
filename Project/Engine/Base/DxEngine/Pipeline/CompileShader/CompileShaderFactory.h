#pragma once
#include <memory>
#include <string>
#include <cassert>

#include "CompileShaderBase.h"
#include "PipelineStruct.h"

namespace NumaEngine {


class CompileShaderFactory
{
public:

	static D3D12_SHADER_BYTECODE& GetCompileShader_VS(
		PipelineType type,
		ComPtr<IDxcUtils> dxcUtils,
		ComPtr<IDxcCompiler3>& dxcCompiler,
		ComPtr<IDxcIncludeHandler> includeHandler
	);

	static D3D12_SHADER_BYTECODE& GetCompileShader_PS(
		PipelineType type,
		ComPtr<IDxcUtils> dxcUtils,
		ComPtr<IDxcCompiler3>& dxcCompiler,
		ComPtr<IDxcIncludeHandler> includeHandler,
		PostEffectType effectType = PostEffectType::kNone
	);

	static D3D12_SHADER_BYTECODE CreateCompileShader(
		const std::wstring& filePath,
		const wchar_t* profile,
		ComPtr<IDxcUtils> dxcUtils,
		ComPtr<IDxcCompiler3> dxcCompiler,
		ComPtr<IDxcIncludeHandler> includeHandler
	);

private:

	static ComPtr<IDxcBlob> CompileShader(
		const std::wstring& filePath,
		const wchar_t* profile,
		ComPtr<IDxcUtils> dxcUtils,
		ComPtr<IDxcCompiler3> dxcCompiler,
		ComPtr<IDxcIncludeHandler> includeHandler
	);

	static std::unique_ptr<CompileShaderBase> GetCompileShaderPtr(
		PipelineType type,
		ComPtr<IDxcUtils> dxcUtils,
		ComPtr<IDxcCompiler3>& dxcCompiler,
		ComPtr<IDxcIncludeHandler> includeHandler
	);

};

} // namespace NumaEngine




// ============================================================
// Object3d
// ============================================================

class Object3dCompileShader : public ::NumaEngine::CompileShaderBase
{
public:

	Object3dCompileShader(
		ComPtr<IDxcUtils> dxcUtils,
		ComPtr<IDxcCompiler3>& dxcCompiler,
		ComPtr<IDxcIncludeHandler> includeHandler
) : ::NumaEngine::CompileShaderBase(dxcUtils, dxcCompiler, includeHandler) {}

	D3D12_SHADER_BYTECODE& BuildVS_Shader() override
	{
     this->vsBytecode = ::NumaEngine::CompileShaderFactory::CreateCompileShader(
			L"resources/shaders/Object3d.VS.hlsl", L"vs_6_0", 
			this->dxcUtils_, this->dxcCompiler_, this->includeHandler_
		);
		return vsBytecode;
	}

	D3D12_SHADER_BYTECODE& BuildPS_Shader(PostEffectType type = PostEffectType::kNone) override
	{
     this->psBytecode = ::NumaEngine::CompileShaderFactory::CreateCompileShader(
			L"resources/shaders/Object3d.PS.hlsl", L"ps_6_0",
			this->dxcUtils_, this->dxcCompiler_, this->includeHandler_
		);
		return psBytecode;
	}
};

// ============================================================
// Sprite
// ============================================================

class SpriteCompileShader : public ::NumaEngine::CompileShaderBase
{
public:

	SpriteCompileShader(
		ComPtr<IDxcUtils> dxcUtils,
		ComPtr<IDxcCompiler3>& dxcCompiler,
		ComPtr<IDxcIncludeHandler> includeHandler
) : ::NumaEngine::CompileShaderBase(dxcUtils, dxcCompiler, includeHandler) {}

	D3D12_SHADER_BYTECODE& BuildVS_Shader() override
	{
     this->vsBytecode = ::NumaEngine::CompileShaderFactory::CreateCompileShader(
			L"resources/shaders/Sprite.VS.hlsl", L"vs_6_0",
			this->dxcUtils_, this->dxcCompiler_, this->includeHandler_
		);
		return this->vsBytecode;
	}

	D3D12_SHADER_BYTECODE& BuildPS_Shader(PostEffectType type = PostEffectType::kNone) override
	{
     this->psBytecode = ::NumaEngine::CompileShaderFactory::CreateCompileShader(
			L"resources/shaders/Sprite.PS.hlsl", L"ps_6_0",
			this->dxcUtils_, this->dxcCompiler_, this->includeHandler_
		);
		return this->psBytecode;
	}
};

// ============================================================
// Line3d
// ============================================================

class Line3dCompileShader : public ::NumaEngine::CompileShaderBase
{
public:

	Line3dCompileShader(
		ComPtr<IDxcUtils> dxcUtils,
		ComPtr<IDxcCompiler3>& dxcCompiler,
		ComPtr<IDxcIncludeHandler> includeHandler
) : ::NumaEngine::CompileShaderBase(dxcUtils, dxcCompiler, includeHandler) {}

	D3D12_SHADER_BYTECODE& BuildVS_Shader() override
	{
     this->vsBytecode = ::NumaEngine::CompileShaderFactory::CreateCompileShader(
			L"resources/shaders/Line3d.VS.hlsl", L"vs_6_0",
			this->dxcUtils_, this->dxcCompiler_, this->includeHandler_
		);
		return this->vsBytecode;
	}

	D3D12_SHADER_BYTECODE& BuildPS_Shader(PostEffectType type = PostEffectType::kNone) override
	{
     this->psBytecode = ::NumaEngine::CompileShaderFactory::CreateCompileShader(
			L"resources/shaders/Line3d.PS.hlsl", L"ps_6_0",
			this->dxcUtils_, this->dxcCompiler_, this->includeHandler_
		);
		return this->psBytecode;
	}
};

// ============================================================
// Particle
// ============================================================

class ParticleCompileShader : public ::NumaEngine::CompileShaderBase
{
public:

	ParticleCompileShader(
		ComPtr<IDxcUtils> dxcUtils,
		ComPtr<IDxcCompiler3>& dxcCompiler,
		ComPtr<IDxcIncludeHandler> includeHandler
) : ::NumaEngine::CompileShaderBase(dxcUtils, dxcCompiler, includeHandler) {}

	D3D12_SHADER_BYTECODE& BuildVS_Shader() override
	{
     this->vsBytecode = ::NumaEngine::CompileShaderFactory::CreateCompileShader(
			L"resources/shaders/Particle.VS.hlsl", L"vs_6_0",
			this->dxcUtils_, this->dxcCompiler_, this->includeHandler_
		);
		return this->vsBytecode;
	}

	D3D12_SHADER_BYTECODE& BuildPS_Shader(PostEffectType type = PostEffectType::kNone) override
	{
     this->psBytecode = ::NumaEngine::CompileShaderFactory::CreateCompileShader(
			L"resources/shaders/Particle.PS.hlsl", L"ps_6_0",
			this->dxcUtils_, this->dxcCompiler_, this->includeHandler_
		);
		return this->psBytecode;
	}
};

// ============================================================
// PrimitiveDrawr
// ============================================================

class PrimitiveDrawrCompileShader : public ::NumaEngine::CompileShaderBase
{
public:

	PrimitiveDrawrCompileShader(
		ComPtr<IDxcUtils> dxcUtils,
		ComPtr<IDxcCompiler3>& dxcCompiler,
		ComPtr<IDxcIncludeHandler> includeHandler
) : ::NumaEngine::CompileShaderBase(dxcUtils, dxcCompiler, includeHandler) {}

	D3D12_SHADER_BYTECODE& BuildVS_Shader() override
	{
     vsBytecode = ::NumaEngine::CompileShaderFactory::CreateCompileShader(
			L"resources/shaders/PrimitiveDrawr.VS.hlsl", L"vs_6_0",
			dxcUtils_, dxcCompiler_, includeHandler_
		);
		return vsBytecode;
	}

	D3D12_SHADER_BYTECODE& BuildPS_Shader(PostEffectType type = PostEffectType::kNone) override
	{
     psBytecode = ::NumaEngine::CompileShaderFactory::CreateCompileShader(
			L"resources/shaders/PrimitiveDrawr.PS.hlsl", L"ps_6_0",
			dxcUtils_, dxcCompiler_, includeHandler_
		);
		return psBytecode;
	}
};

// ============================================================
// RenderTexture
// ============================================================

class RenderTextureCompileShader : public ::NumaEngine::CompileShaderBase
{
public:

	RenderTextureCompileShader(
		ComPtr<IDxcUtils> dxcUtils,
		ComPtr<IDxcCompiler3>& dxcCompiler,
		ComPtr<IDxcIncludeHandler> includeHandler
) : ::NumaEngine::CompileShaderBase(dxcUtils, dxcCompiler, includeHandler) {}

	D3D12_SHADER_BYTECODE& BuildVS_Shader() override
	{
     this->vsBytecode = ::NumaEngine::CompileShaderFactory::CreateCompileShader(
			L"resources/shaders/Fullscreen.VS.hlsl", L"vs_6_0",
			this->dxcUtils_, this->dxcCompiler_, this->includeHandler_
		);
		return this->vsBytecode;
	}

	D3D12_SHADER_BYTECODE& BuildPS_Shader(PostEffectType type = PostEffectType::kNone) override
	{
		switch (type)
		{
		case PostEffectType::kNone:
			break;
		case PostEffectType::kRenderTexture:
     this->psBytecode = ::NumaEngine::CompileShaderFactory::CreateCompileShader(
			L"resources/shaders/CopyImage.PS.hlsl", L"ps_6_0",
			this->dxcUtils_, this->dxcCompiler_, this->includeHandler_
		);
			break;
		case PostEffectType::kGrayscale:
     this->psBytecode = ::NumaEngine::CompileShaderFactory::CreateCompileShader(
			L"resources/shaders/Grayscale.PS.hlsl", L"ps_6_0",
			this->dxcUtils_, this->dxcCompiler_, this->includeHandler_
		);
			break;
		case PostEffectType::kVignette:
     this->psBytecode = ::NumaEngine::CompileShaderFactory::CreateCompileShader(
			L"resources/shaders/Vignette.PS.hlsl", L"ps_6_0",
			this->dxcUtils_, this->dxcCompiler_, this->includeHandler_
		);
			break;
        case PostEffectType::kSmoothing:
		this->psBytecode = ::NumaEngine::CompileShaderFactory::CreateCompileShader(
			L"resources/shaders/GaussianFilter.PS.hlsl", L"ps_6_0",
			this->dxcUtils_, this->dxcCompiler_, this->includeHandler_
		);
			break;
		case PostEffectType::kOutLine:
       this->psBytecode = ::NumaEngine::CompileShaderFactory::CreateCompileShader(
			L"resources/shaders/LuminanceBasedOutline.PS.hlsl", L"ps_6_0",
			this->dxcUtils_, this->dxcCompiler_, this->includeHandler_
		);
			break;
		default:
			break;
		}
		
		return psBytecode;
	}
};

// ============================================================
// Animation
// ============================================================

class AnimationCompileShader : public ::NumaEngine::CompileShaderBase
{
public:

	AnimationCompileShader(
		ComPtr<IDxcUtils> dxcUtils,
		ComPtr<IDxcCompiler3>& dxcCompiler,
		ComPtr<IDxcIncludeHandler> includeHandler
) : ::NumaEngine::CompileShaderBase(dxcUtils, dxcCompiler, includeHandler) {}

	D3D12_SHADER_BYTECODE& BuildVS_Shader() override
	{
     this->vsBytecode = ::NumaEngine::CompileShaderFactory::CreateCompileShader(
			L"resources/shaders/SkinningObject3d.VS.hlsl", L"vs_6_0",
			this->dxcUtils_, this->dxcCompiler_, this->includeHandler_
		);
		return this->vsBytecode;
	}

	D3D12_SHADER_BYTECODE& BuildPS_Shader(PostEffectType type = PostEffectType::kNone) override
	{
   this->psBytecode = ::NumaEngine::CompileShaderFactory::CreateCompileShader(
			L"resources/shaders/Object3d.PS.hlsl", L"ps_6_0",
			this->dxcUtils_, this->dxcCompiler_, this->includeHandler_
		);
		return this->psBytecode;
	}
};

// ============================================================
// Skybox
// ============================================================

class SkyboxCompileShader : public ::NumaEngine::CompileShaderBase
{
public:

	SkyboxCompileShader(
		ComPtr<IDxcUtils> dxcUtils,
		ComPtr<IDxcCompiler3>& dxcCompiler,
		ComPtr<IDxcIncludeHandler> includeHandler
) : ::NumaEngine::CompileShaderBase(dxcUtils, dxcCompiler, includeHandler) {
	}

	D3D12_SHADER_BYTECODE& BuildVS_Shader() override
	{
    vsBytecode = ::NumaEngine::CompileShaderFactory::CreateCompileShader(
			L"resources/shaders/Skybox.VS.hlsl", L"vs_6_0",
			dxcUtils_, dxcCompiler_, includeHandler_
		);
		return vsBytecode;
	}

	D3D12_SHADER_BYTECODE& BuildPS_Shader(PostEffectType type = PostEffectType::kNone) override
	{
    psBytecode = ::NumaEngine::CompileShaderFactory::CreateCompileShader(
			L"resources/shaders/Skybox.PS.hlsl", L"ps_6_0",
			dxcUtils_, dxcCompiler_, includeHandler_
		);
		return psBytecode;
	}
};

// ============================================================
// ObjectOutLineMask
// ============================================================

class ObjectOutLineMaskCompileShader : public ::NumaEngine::CompileShaderBase
{
public:

	ObjectOutLineMaskCompileShader(
		ComPtr<IDxcUtils> dxcUtils,
		ComPtr<IDxcCompiler3>& dxcCompiler,
		ComPtr<IDxcIncludeHandler> includeHandler
) : ::NumaEngine::CompileShaderBase(dxcUtils, dxcCompiler, includeHandler) {
	}

	D3D12_SHADER_BYTECODE& BuildVS_Shader() override
	{
    vsBytecode = ::NumaEngine::CompileShaderFactory::CreateCompileShader(
			L"resources/shaders/Object3d.VS.hlsl", L"vs_6_0",
			dxcUtils_, dxcCompiler_, includeHandler_
		);
		return vsBytecode;
	}

	D3D12_SHADER_BYTECODE& BuildPS_Shader(PostEffectType type = PostEffectType::kNone) override
	{
    psBytecode = ::NumaEngine::CompileShaderFactory::CreateCompileShader(
			L"resources/shaders/OutlineMask.PS.hlsl", L"ps_6_0",
			dxcUtils_, dxcCompiler_, includeHandler_
		);
		return psBytecode;
	}
};

// ============================================================
// AnimationOutLineMask
// ============================================================

class AnimationOutLineMaskCompileShader : public ::NumaEngine::CompileShaderBase
{
public:

	AnimationOutLineMaskCompileShader(
		ComPtr<IDxcUtils> dxcUtils,
		ComPtr<IDxcCompiler3>& dxcCompiler,
		ComPtr<IDxcIncludeHandler> includeHandler
) : ::NumaEngine::CompileShaderBase(dxcUtils, dxcCompiler, includeHandler) {
	}

	D3D12_SHADER_BYTECODE& BuildVS_Shader() override
	{
    vsBytecode = ::NumaEngine::CompileShaderFactory::CreateCompileShader(
			L"resources/shaders/SkinningObject3d.VS.hlsl", L"vs_6_0",
			dxcUtils_, dxcCompiler_, includeHandler_
		);
		return vsBytecode;
	}

	D3D12_SHADER_BYTECODE& BuildPS_Shader(PostEffectType type = PostEffectType::kNone) override
	{
    psBytecode = ::NumaEngine::CompileShaderFactory::CreateCompileShader(
			L"resources/shaders/OutlineMask.PS.hlsl", L"ps_6_0",
			dxcUtils_, dxcCompiler_, includeHandler_
		);
		return psBytecode;
	}
};

// ============================================================
// ObjectShadowMapDepth
// ============================================================

class ObjectShadowMapDepthCompileShader : public ::NumaEngine::CompileShaderBase
{
public:

	ObjectShadowMapDepthCompileShader(
		ComPtr<IDxcUtils> dxcUtils,
		ComPtr<IDxcCompiler3>& dxcCompiler,
		ComPtr<IDxcIncludeHandler> includeHandler
) : ::NumaEngine::CompileShaderBase(dxcUtils, dxcCompiler, includeHandler) {
	}

	D3D12_SHADER_BYTECODE& BuildVS_Shader() override
	{
    vsBytecode = ::NumaEngine::CompileShaderFactory::CreateCompileShader(
			L"resources/shaders/ObjectShadowMap.VS.hlsl", L"vs_6_0",
			dxcUtils_, dxcCompiler_, includeHandler_
		);
		return vsBytecode;
	}

	D3D12_SHADER_BYTECODE& BuildPS_Shader(PostEffectType type = PostEffectType::kNone) override
	{
    psBytecode = ::NumaEngine::CompileShaderFactory::CreateCompileShader(
			L"resources/shaders/OutlineMask.PS.hlsl", L"ps_6_0",
			dxcUtils_, dxcCompiler_, includeHandler_
		);
		return psBytecode;
	}
};

// ============================================================
// AnimationShadowMapDepth
// ============================================================

class AnimationShadowMapDepthCompileShader : public ::NumaEngine::CompileShaderBase
{
public:

	AnimationShadowMapDepthCompileShader(
		ComPtr<IDxcUtils> dxcUtils,
		ComPtr<IDxcCompiler3>& dxcCompiler,
		ComPtr<IDxcIncludeHandler> includeHandler
) : ::NumaEngine::CompileShaderBase(dxcUtils, dxcCompiler, includeHandler) {
	}

	D3D12_SHADER_BYTECODE& BuildVS_Shader() override
	{
         vsBytecode = ::NumaEngine::CompileShaderFactory::CreateCompileShader(
			L"resources/shaders/AnimationShadowMap.VS.hlsl", L"vs_6_0",
			dxcUtils_, dxcCompiler_, includeHandler_
		);
		return vsBytecode;
	}

	D3D12_SHADER_BYTECODE& BuildPS_Shader(PostEffectType type = PostEffectType::kNone) override
	{
         psBytecode = ::NumaEngine::CompileShaderFactory::CreateCompileShader(
			L"resources/shaders/OutlineMask.PS.hlsl", L"ps_6_0",
			dxcUtils_, dxcCompiler_, includeHandler_
		);
		return psBytecode;
	}
};

// ============================================================
// SpriteDissolve
// ============================================================

class SpriteDissolveCompileShader : public ::NumaEngine::CompileShaderBase
{
public:

	SpriteDissolveCompileShader(
		ComPtr<IDxcUtils> dxcUtils,
		ComPtr<IDxcCompiler3>& dxcCompiler,
		ComPtr<IDxcIncludeHandler> includeHandler
) : ::NumaEngine::CompileShaderBase(dxcUtils, dxcCompiler, includeHandler) {
	}

	D3D12_SHADER_BYTECODE& BuildVS_Shader() override
	{
     vsBytecode = ::NumaEngine::CompileShaderFactory::CreateCompileShader(
			L"resources/shaders/Sprite.VS.hlsl", L"vs_6_0",
			dxcUtils_, dxcCompiler_, includeHandler_
		);
		return vsBytecode;
	}

	D3D12_SHADER_BYTECODE& BuildPS_Shader(PostEffectType type = PostEffectType::kNone) override
	{
     psBytecode = ::NumaEngine::CompileShaderFactory::CreateCompileShader(
			L"resources/shaders/SpriteDissolve.PS.hlsl", L"ps_6_0",
			dxcUtils_, dxcCompiler_, includeHandler_
		);
		return psBytecode;
	}
};