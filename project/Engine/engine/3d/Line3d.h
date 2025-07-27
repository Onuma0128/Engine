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

	void Initialize(const std::vector<Vector3>& positions, uint32_t maxLinesReserve = 0);

	void Update();

	void Draws();

	/*==================== メンバ関数 ====================*/
private:
	
	void CreateLocalVB();
	void CreateInstanceVB(const std::vector<Vector3>& pos, uint32_t capacity);
	void CreateCB();

	/*==================== アクセッサ ====================*/
public:

	void SetPositions(const std::vector<Vector3>& positions);
	void SetColor(const Vector3& color);

	RenderOptions& GetRenderOptions() { return renderOptions_; }
	bool GetIsMultiple()const { return isMultiple_; }

	std::vector<Vector3> CreateBox(const Vector3& min, const Vector3& max);
	std::vector<Vector3> CreateSphere(const float& radius);
	std::vector<Vector3> CreateGrid(float cell, float size, float y = 0.0f);


private:

	struct LocalVertex { float t; };
	struct LineInstance { Vector3 startPos; Vector3 endPos; };

	std::unique_ptr<Line3dBase> line3dBase_ = nullptr;

	ComPtr<ID3D12Resource> localVB_ = nullptr;
	ComPtr<ID3D12Resource> instanceVB_ = nullptr;
	ComPtr<ID3D12Resource> wvpResource_ = nullptr;
	ComPtr<ID3D12Resource> materialResource_ = nullptr;

	D3D12_VERTEX_BUFFER_VIEW vbvLocal_{};
	D3D12_VERTEX_BUFFER_VIEW vbvInst_{};
	Matrix4x4* wvpData_ = nullptr;
	Vector4* color_ = nullptr;

	uint32_t lineCount_ = 0;
	uint32_t capacityLines_ = 0;

	// 描画するか
	RenderOptions renderOptions_;
	bool isMultiple_ = false;

};

