#pragma once

#define NOMINMAX
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include "wrl.h"
#include <memory>
#include <vector>

#include "Line3dBase.h"

#include "ModelStruct.h"
#include "Vector3.h"
#include "Vector4.h"

using Microsoft::WRL::ComPtr;

class Line3d
{
public:

	void Initialize(const std::vector<Vector3>& positions);

	void Update();

private:

	// マテリアルデータの作成
	void MakeMaterialData();

public:
	/*==================== アクセッサ ====================*/

	Material& GetMaterial() { return materialData_; }

	void SetPositions(const std::vector<Vector3>& positions);
	void SetColor(const Vector3& color);

	std::vector<Vector3> CreateBox(const Vector3& min, const Vector3& max);
	std::vector<Vector3> CreateSphere(const float& radius);
	std::vector<Vector3> CreateSegment(const Vector3& start, const Vector3& direction);
	std::vector<Vector3> CreateGrid(float cell, float size, float y = 0.0f);

private:

	/*==================== マテリアル ====================*/

	Material materialData_;

	uint32_t lineIndex_ = 0;

};

