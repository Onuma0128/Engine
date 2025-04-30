#pragma once

#include "RootSignatureBase.h"
#include "PipelineStruct.h"

class RootSignatureFactory
{
public:

	static ComPtr<ID3D12RootSignature> GetRootSignature(PipelineType type, ComPtr<ID3D12Device> device);

};

