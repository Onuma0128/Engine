#pragma once
#include "wrl.h"
#include <dxcapi.h>
#pragma comment(lib,"dxcompiler.lib")
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")

using Microsoft::WRL::ComPtr;

class CompileShaderBase
{
public:

	CompileShaderBase(
		ComPtr<IDxcUtils> dxcUtils,
		ComPtr<IDxcCompiler3>& dxcCompiler,
		ComPtr<IDxcIncludeHandler> includeHandler
	);
	virtual ~CompileShaderBase() = default;
	virtual D3D12_SHADER_BYTECODE& BuildVS_Shader() = 0;
	virtual D3D12_SHADER_BYTECODE& BuildPS_Shader() = 0;

protected:

	ComPtr<IDxcUtils> dxcUtils_;
	ComPtr<IDxcCompiler3> dxcCompiler_;
	ComPtr<IDxcIncludeHandler> includeHandler_;

	D3D12_SHADER_BYTECODE vsBytecode{};
	D3D12_SHADER_BYTECODE psBytecode{};

};

