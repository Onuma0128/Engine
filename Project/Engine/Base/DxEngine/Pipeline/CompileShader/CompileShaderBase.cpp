#include "CompileShaderBase.h"

NumaEngine::CompileShaderBase::CompileShaderBase(ComPtr<IDxcUtils> dxcUtils, ComPtr<IDxcCompiler3>& dxcCompiler, ComPtr<IDxcIncludeHandler> includeHandler) :
	dxcUtils_(dxcUtils),
	dxcCompiler_(dxcCompiler),
	includeHandler_(includeHandler) {}
